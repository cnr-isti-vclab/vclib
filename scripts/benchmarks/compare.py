#!/usr/bin/env python3
"""
VCLib Benchmark Comparison Tool
Compares two JSON benchmark reports produced by any VCLib benchmark targets
(e.g., historical vs current version, different compilers, optimization flags, etc.).
"""

import argparse
import json
import math
import os
import re
import shlex
import subprocess
import sys
import tempfile


def format_time(ms: float) -> str:
    if ms >= 1000.0:
        return f"{ms / 1000.0:7.2f} s "
    elif ms >= 1.0:
        return f"{ms:7.2f} ms"
    elif ms >= 0.001:
        return f"{ms * 1000.0:7.2f} µs"
    else:
        return f"{ms * 1000000.0:7.1f} ns"


def format_ratio(ratio: float) -> str:
    if ratio >= 1000.0:
        return ">1000x"
    if ratio >= 100.0:
        return f"{ratio:.0f}x"
    if ratio < 0.01:
        return "<0.01x"
    return f"{ratio:.2f}x"


def format_speedup(a_ms: float, b_ms: float, use_color: bool = True) -> str:
    if b_ms <= 0:
        return "N/A"
    ratio = a_ms / b_ms
    pct = (a_ms - b_ms) / a_ms * 100.0 if a_ms > 0 else 0.0

    if ratio >= 1.05:
        # B is faster than A
        text = f"{format_ratio(ratio)} (+{pct:.1f}%)"
        color = "\033[92m"  # Green
    elif ratio <= 0.95:
        # A is faster than B
        text = f"{format_ratio(ratio)} ({pct:.1f}%)"
        color = "\033[91m"  # Red
    else:
        text = f"{format_ratio(ratio)} (0.0%)"
        color = "\033[93m"  # Yellow

    return f"{color}{text}\033[0m" if use_color else text


def format_rss(rss_a: float, rss_b: float) -> str:
    if rss_a <= 0 or rss_b <= 0:
        return f"{rss_a:.1f} / {rss_b:.1f} MB"
    delta_pct = (rss_b - rss_a) / rss_a * 100.0
    sign = "+" if delta_pct > 0 else ""
    return f"{rss_a:.1f} / {rss_b:.1f} MB ({sign}{delta_pct:.1f}%)"


def visible_len(text: str) -> int:
    return len(re.sub(r"\033\[[0-9;]*m", "", text))


def pad(text: str, width: int, left: bool = False) -> str:
    fill = " " * max(0, width - visible_len(text))
    return text + fill if left else fill + text


def split_group(name: str):
    """Splits 'a/b/c' in ('a/b', 'c'); names without '/' have no group."""
    if "/" in name:
        group, short = name.rsplit("/", 1)
        return group, short
    return "", name


def compare_reports(data_a: dict, data_b: dict, label_a: str = None, label_b: str = None, use_color: bool = True):
    name_a = label_a or data_a.get("version") or "A"
    name_b = label_b or data_b.get("version") or "B"

    benches_a = {b["name"]: b for b in data_a.get("benchmarks", [])}
    benches_b = {b["name"]: b for b in data_b.get("benchmarks", [])}

    dataset_a = data_a.get("dataset", "Unknown")
    dataset_b = data_b.get("dataset", "Unknown")
    dataset_str = dataset_a if dataset_a == dataset_b else f"{dataset_a} vs {dataset_b}"

    # Metadata summary
    meta_a = data_a.get("metadata", {})
    meta_str = ""
    if meta_a:
        meta_items = [f"{k}: {v:,}" if isinstance(v, (int, float)) else f"{k}: {v}" for k, v in meta_a.items()]
        meta_str = f" ({', '.join(meta_items)})"

    headers = [
        "Benchmark Name",
        f"{name_a} Median",
        f"{name_b} Median",
        f"Speedup ({name_b}/{name_a})",
        f"Peak RSS ({name_a}/{name_b})",
    ]

    all_names = list(benches_a.keys())
    for name in benches_b.keys():
        if name not in all_names:
            all_names.append(name)

    # Benchmarks are grouped by the path of their name (e.g. stat/topology)
    groups = {}
    for name in all_names:
        group, short = split_group(name)
        groups.setdefault(group, []).append((name, short))

    speedups = []
    ignored = 0
    # each entry is either a group title (str) or a row (list of 5 str)
    lines = []

    for group, items in groups.items():
        if group:
            lines.append(group)
        for name, short in items:
            display = f"  {short}" if group else short
            ba = benches_a.get(name)
            bb = benches_b.get(name)

            if ba is None:
                lines.append([display, "--", format_time(bb["time_median_ms"]), f"{name_b} only", ""])
                continue
            if bb is None:
                lines.append([display, format_time(ba["time_median_ms"]), "--", f"{name_a} only", ""])
                continue

            ta = ba["time_median_ms"]
            tb = bb["time_median_ms"]
            # benchmarks below 1 µs on both sides are just noise
            if tb > 0 and max(ta, tb) >= 0.001:
                speedups.append(ta / tb)
            else:
                ignored += 1

            lines.append(
                [
                    display,
                    format_time(ta),
                    format_time(tb),
                    format_speedup(ta, tb, use_color=use_color),
                    format_rss(ba.get("peak_rss_mb", 0.0), bb.get("peak_rss_mb", 0.0)),
                ]
            )

    rows = [line for line in lines if not isinstance(line, str)]
    widths = [visible_len(h) for h in headers]
    for row in rows:
        for i, cell in enumerate(row):
            widths[i] = max(widths[i], visible_len(cell))

    title = f"VCLib Benchmark Comparison: {name_a} vs {name_b}"
    dataset_line = f"Dataset: {dataset_str}{meta_str}"
    total = max(sum(widths) + 2 * (len(widths) - 1), len(title), len(dataset_line))

    def format_row(row):
        cells = [pad(row[0], widths[0], left=True)]
        cells += [pad(cell, widths[i]) for i, cell in enumerate(row[1:], start=1)]
        return "  ".join(cells).rstrip()

    print("=" * total)
    print(title)
    print(dataset_line)
    print("=" * total)
    print(format_row(headers))
    print("-" * total)

    for line in lines:
        if isinstance(line, str):
            print(f"\033[1m{line}\033[0m" if use_color else line)
        else:
            print(format_row(line))

    print("=" * total)
    if speedups:
        geom_mean = math.exp(sum(math.log(s) for s in speedups) / len(speedups))
        print(f"Geometric Mean Speedup: {geom_mean:.2f}x ({name_b} relative to {name_a}) across {len(speedups)} benchmarks")
        if ignored:
            print(f"(benchmarks faster than 1 µs in both reports are not included: {ignored})")
        print("=" * total)


def main():
    parser = argparse.ArgumentParser(
        description="VCLib Generic Benchmark Comparison Tool. Compares two benchmark JSON reports."
    )
    parser.add_argument("report_a", nargs="?", help="Path to first JSON benchmark report (baseline)")
    parser.add_argument("report_b", nargs="?", help="Path to second JSON benchmark report (comparison)")
    parser.add_argument("--bin-a", help="Path to first benchmark executable")
    parser.add_argument("--bin-b", help="Path to second benchmark executable")
    parser.add_argument("--args", default="", help="Additional arguments forwarded to both benchmark binaries")
    parser.add_argument("--label-a", help="Custom label for report/binary A")
    parser.add_argument("--label-b", help="Custom label for report/binary B")
    parser.add_argument("--iterations", type=int, default=5, help="Number of benchmark iterations")
    parser.add_argument("--no-color", action="store_true", help="Disable colored output")

    args = parser.parse_args()
    use_color = sys.stdout.isatty() and not args.no_color

    # Mode 1: Run two binaries directly
    if args.bin_a and args.bin_b:
        with tempfile.NamedTemporaryFile(suffix=".json", delete=False) as f_a, \
             tempfile.NamedTemporaryFile(suffix=".json", delete=False) as f_b:
            path_a = f_a.name
            path_b = f_b.name

        try:
            extra = shlex.split(args.args)
            cmd_a = [args.bin_a, "--json", path_a, "--iterations", str(args.iterations)] + extra
            cmd_b = [args.bin_b, "--json", path_b, "--iterations", str(args.iterations)] + extra

            label_a = args.label_a or "A"
            label_b = args.label_b or "B"

            print(f">>> Running Benchmark [{label_a}]: {' '.join(cmd_a)}")
            subprocess.run(cmd_a, check=True)

            print(f"\n>>> Running Benchmark [{label_b}]: {' '.join(cmd_b)}")
            subprocess.run(cmd_b, check=True)

            with open(path_a, "r") as fa:
                data_a = json.load(fa)
            with open(path_b, "r") as fb:
                data_b = json.load(fb)

            print("\n")
            compare_reports(data_a, data_b, label_a=args.label_a, label_b=args.label_b, use_color=use_color)
        finally:
            if os.path.exists(path_a):
                os.remove(path_a)
            if os.path.exists(path_b):
                os.remove(path_b)
        return

    # Mode 2: Compare two existing JSON report files
    if args.report_a and args.report_b:
        with open(args.report_a, "r") as fa:
            data_a = json.load(fa)
        with open(args.report_b, "r") as fb:
            data_b = json.load(fb)

        compare_reports(data_a, data_b, label_a=args.label_a, label_b=args.label_b, use_color=use_color)
        return

    parser.print_help()
    sys.exit(1)


if __name__ == "__main__":
    main()

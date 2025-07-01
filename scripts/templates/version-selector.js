/**
 * Version Selector Script for VCLib Documentation
 * 
 * This script dynamically discovers available documentation versions
 * by checking which directories exist at the same level as the current version,
 * and populates a dropdown menu for version selection.
 */

(function() {
    'use strict';

    /**
     * Get the current version from the URL path
     * @returns {string} Current version (e.g., 'devel', 'v1.0.0')
     */
    function getCurrentVersion() {
        const path = window.location.pathname;
        const segments = path.split('/').filter(segment => segment !== '');
        
        // For a path like "/devel/" or "/v0.0.1/somepage.html"
        // The first non-empty segment should be the version
        if (segments.length > 0) {
            return segments[0];
        }
        
        return 'unknown';
    }

    /**
     * Get the base URL for version switching
     * @returns {string} Base URL up to the version part
     */
    function getBaseUrl() {
        const path = window.location.pathname;
        const segments = path.split('/').filter(segment => segment !== '');
        
        // For a path like "/devel/" or "/v0.0.1/somepage.html"
        // We want to return the base path without the version
        // So for "/devel/somepage.html" we return "/"
        
        if (segments.length > 0) {
            // Remove the first segment (version) and reconstruct the path
            const baseSegments = segments.slice(0, -segments.length);
            return '/';
        }
        
        return '/';
    }

    /**
     * Load available versions from the versions.json file
     * @returns {Promise<Array<string>>} Array of available versions
     */
    async function loadVersions() {
        try {
            // The versions.json file is in the parent directory of all versions
            // So from "/devel/" we need to go to "/versions.json"
            const response = await fetch('/versions.json', {
                cache: 'no-cache'
            });
            
            if (!response.ok) {
                throw new Error(`Failed to load versions: ${response.status}`);
            }
            
            const data = await response.json();
            return data.versions || [];
        } catch (error) {
            console.warn('Failed to load versions from versions.json:', error);
            return [];
        }
    }

    /**
     * Populate the version dropdown with discovered versions
     * @param {Array<string>} versions Available versions
     */
    function populateVersionDropdown(versions) {
        const dropdown = document.getElementById('version-dropdown');
        if (!dropdown) return;
        
        const currentVersion = getCurrentVersion();
        
        // Clear existing options
        dropdown.innerHTML = '';
        
        // Add versions to dropdown
        versions.forEach(version => {
            const option = document.createElement('option');
            option.value = version;
            option.textContent = version === 'devel' ? 'Development' : version;
            
            if (version === currentVersion) {
                option.selected = true;
            }
            
            dropdown.appendChild(option);
        });
        
        // Add change event listener
        dropdown.addEventListener('change', function() {
            const selectedVersion = this.value;
            if (selectedVersion && selectedVersion !== currentVersion) {
                switchToVersion(selectedVersion);
            }
        });
    }

    /**
     * Switch to a different version
     * @param {string} version Target version
     */
    function switchToVersion(version) {
        const currentPath = window.location.pathname;
        const currentVersion = getCurrentVersion();
        
        console.log('switchToVersion - Target version:', version);
        console.log('switchToVersion - Current path:', currentPath);
        console.log('switchToVersion - Current version:', currentVersion);
        
        // Replace current version with new version in the path
        const newPath = currentPath.replace(
            `/${currentVersion}/`,
            `/${version}/`
        );
        
        console.log('switchToVersion - New path:', newPath);
        
        // Navigate to the new version
        console.log('switchToVersion - Navigating to:', newPath);
        window.location.href = newPath;
    }

    /**
     * Initialize the version selector
     */
    async function initVersionSelector() {
        try {
            const versions = await loadVersions();
            
            if (versions.length > 1) {
                populateVersionDropdown(versions);
            } else {
                // Hide the version selector if there's only one version
                const versionSelector = document.getElementById('version-selector');
                if (versionSelector) {
                    versionSelector.style.display = 'none';
                }
            }
        } catch (error) {
            console.warn('Failed to initialize version selector:', error);
            
            // Fallback: hide the selector
            const versionSelector = document.getElementById('version-selector');
            if (versionSelector) {
                versionSelector.style.display = 'none';
            }
        }
    }

    // Initialize when the DOM is ready
    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', initVersionSelector);
    } else {
        initVersionSelector();
    }

})();

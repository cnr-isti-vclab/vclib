## TODO
- Face selection (Requires a separate buffer) (Selecting faces should also select vertices; but not the other way around)
- Edge selection (This may not be possible yet because @bigmat is still working on a proper edge implementation)
- Maybe change code so that the selection box uniform is already in NDC space?
- Non penetrating selection, possible ways:
    1. Depth buffer interpolation (AWFUL, DO NOT DO THIS)
        1. Calculate Depth Buffer (framebuffer)
        2. Calculate position of point in screen space
        3. Interpolate bilinearly the depth value within (pSS.x - 0.5, pSS.y - 0.5) and (pSS.x + 0.5, pSS.y + 0.5) 
        (Likely done automatically when reading the texture) 
        (Transform those coordinates in texture space, right now they are in Screen Space)
        (If possible ignore values equal to one (unless the others are near it), because there's likely nothing there)
        4. If point depth (NDC) is within an epsilon of said depth, it is visible; otherwise it isn't
        5. Select the points that are visible and that are within the selection box
    2. Space subdivision bitmap:
        1. Create an IndexBuffer (uint) of a certain size^3
        2. Generate the primitives (triangles, do not do this for point clouds) and set the bit in the buffer to 1 if the corresponding cube contains something
        3. When trying to calculate the selection:
            1. Draw line from view point to point you are trying to select
            2. If some cube you pass through (except the final one) contains something then that point is not visible and you do not select it
    3. Space subdivision with depth:
        VARIANT of point 2: use more than 1 bit per cube, if  first bit 0 then empty. If first bit not 0 interpret remaining bits as a depth (from 0 closest to 1 farthest). If depth of point is within a certain 
        delta or closer than it then select it otherwise don't
    4. Space subdivision tree-like bitmap (basically Binary Space Partitioning):
        VARIANT of point 2: use more than 1 bit per cube (either fixed or a variable amount, but a variable amount would remove the constant access time). The first bit indicates if the entire cube contains something, all
        the others indicate if recursively smaller cubes inside that cube contain something
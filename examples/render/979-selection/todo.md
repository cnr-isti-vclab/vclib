## TODO
- Face selection (Requires a separate buffer) (Selecting faces should also select vertices; but not the other way around)
    - Rethink current approach: using the vertex selection to calculate face selection is incorrect: just think about the case in which you have one gigantic triangle and none of the vertices are in the selection box: that triangle should be selected regardless...
- Generalize invert, all and none selection programs: they do the same thing regardless of if it is face or vertex (that is, different modes but same compute shader)
- Edge selection (This may not be possible yet because @bigmat is still working on a proper edge implementation)
- Maybe change code so that the selection box uniform is already in NDC space?
- Some way to manage the copying of buffers to texture given buffers that exceed textureMaxSize*textureMaxSize. (multiple textures? same texture blitted multiple times (each time requires 2 frames though)? who knows.)
- Non penetrating selection, possible ways:
    1. Depth buffer interpolation (Remember: two passes, one with depth_less and one with depth_lequal);
        1. Calculate Depth Buffer (framebuffer)
        2. Calculate position of point in screen space
        3. Interpolate bilinearly the depth value within (pSS.x - 0.5, pSS.y - 0.5) and (pSS.x + 0.5, pSS.y + 0.5) 
        (Likely done automatically when reading the texture) 
        (Transform those coordinates in texture space, right now they are in Screen Space)
        (If possible ignore values equal to one (unless the others are near it), because there's likely nothing there)
        4. If point depth (NDC) is within an epsilon of said depth, it is visible; otherwise it isn't
        5. Select the points that are visible and that are within the selection box
- Selection per poly for poly meshes:
    - Requires 3 buffers:
        1. The triangle selection buffer (the same used for regular face selection)
        2. The "poly grouping indexing buffer" (parallel to the previous one), 2 values (start, end) used to index the next buffer
        3. The "poly triangle grouping buffer", that contains triangle indices grouped by poly (indices of triangles that belong to the same poly are contiguous)
    - Whenever a triangle is INSIDE the selection:
        1. Get the "poly triangle grouping buffer" indices from the "poly grouping indexing buffer"
        2. Use said indices to get all the triangle indices in the "poly triangle grouping buffer"
        3. Set the bits of the triangles corresponding to the indices in the triangle selection buffer to 1
    - Every time you do a regular selection you have to first set the triangle selection buffer to 0s first (so basically you replace the "regular selection" with an "add selection")
## TODO
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
- When you init a view you have to:
    - Set the framebuffer
    - Set the viewRect
    - Set all the transforms
    - Touch the view
    - FOR MORE INFORMATION SEE context.cpp



- Finish visible color based
- Transfer selection buffer to flags (and viceversa, on load)
- Visible selection with:
    - Depth LT pass
    - Depth LE pass which writes the selection buffer

- OPTIONALS:
    - Linear depth instead of log
    
- To manage distant visible selections:
    - Given a mesh's bounding box, i want to find a sub-projection that includes "perfectly" said bounding box (this means
    that i have to project the box in screen space and build the new projection from there)
    - +1 if you intersect the selection box with the sub-projection and only render the resulting sub-sub-box

- BUG: currently the visible selection does NOT work because the final render pass is on viewId 0 and the render pass i do for visible selection is on a viewId after that (by default bgfx uses the viewIds in increasing order)
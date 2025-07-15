Student Name:
Kuek Wei Jie
Special Directions (if any):
Robust geometric line-of-sight (LOS) logic implemented as per the project PDF. Wall boundaries include a small epsilon to prevent LOS skimming. Diagonal blocking for adjacent visible cells applied to prevent corner leaks. Propagation and normalization strictly follow the provided algorithm. No grid-stepping or raster-based shortcuts used.
Missing features (if any):
Extra credit not done 
My experience working on this project:
Although it was just implementing set functions, the pseudo code is not direct so it made me do some research and understand the purpose and specificity of each functions. I got stuck at issues for visibility for agent where it turns out its not the clear path function fault, but instead the visibility of the cell check. The enemy AI behavior, including FOV, detection radius, and player-seeking logic, required careful attention to world-space calculations and occupancy propagation. Overall, this project reinforced my understanding of grid-based maps, visibility algorithms, and the importance of strict adherence to project specifications. 
Hours spent:
22 hours

Extra credits:N/A
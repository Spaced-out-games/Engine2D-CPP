// Manages 2D and 3D hitboxes alike, and handles collision checks performed on the GPU (for vectorized collision checking
// Make hitboxes have private constructors with hb_manager as a friend. hb_manager will add new hitboxes to its std::vector and returns a pointer to the hitbox.
// Better yet, have hb_manager inherit from ent_manager and have its own special method that invokes either the CPU or GPU, depending on availability
// This way, we arent repeating ourselves and have an efficient insertion / deletion / organization method.
// It also manages an hb_grid and assigns each accordingly
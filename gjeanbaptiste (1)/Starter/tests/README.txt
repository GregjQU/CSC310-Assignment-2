Naming:
	<algorithm>_<requests>_<success>.txt

Where the <requests> are:
	m -> malloc
	f -> free
	r -> realloc

And the <success> is either:
	g -> No error (good)
	b -> Error (bad)

This means that best_fm_g.txt is a test of the best-first algorithm
using only free and malloc requests, and the simulation has no errors.

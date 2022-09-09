# Contact-Process

C code for a general stochastic model of spatial spread. It consist of a lattice population of replicating particles which can die and/or colonize near-by location. Thus, we conside a 2D lattice of spatial locations which can be either vacant (state 0) or occupied (state 1). Latiice sites on these states can locally go exting (die) or, if empty, be colonized by adjacent occupied sited with a probability proportional to the local occupancy.

For a mathematical description of the process

https://en.wikipedia.org/wiki/Contact_process_(mathematics)

For a guide on the mathematical model for ecological applications, see: Durrett and Levin, 1998

https://royalsocietypublishing.org/doi/10.1098/rstb.1994.0028

doi:

https://doi.org/10.1098/rstb.1994.0028





DEPENDENCIES:

---[mt64.h]
Pseudo Random Number Generation (RNG) is achieved using the Mersenne twister algorithm.

https://es.wikipedia.org/wiki/Mersenne_twister

The implementation is thank to the MT19937 algorithm, which for 64 bits:

https://github.com/shivakar/rng-reference/tree/master/MT19937

In particular, here, we use (#include) library mt64.h:

https://github.com/shivakar/rng-reference/blob/master/MT19937/mt64.h

---[gtk]
For the GUI we use Gtk version 3. The plan for the near future is to move to gtk-4 and follow the Gnome app vision.

https://docs.gtk.org/gtk3/

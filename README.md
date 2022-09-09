# Contact-Process

The Contact process is acgeneral stochastic model for the spatial spread
It consist of a lattice population of replicating particles/sites which can die (go extinct locally) and/or, through birth process, colonize near-by vacant locations.

SPACE:

We conside a 2D lattice (periodic boundary conditions; a torus) representing spatial locations which can be either vacant (state 0) or occupied (state 1). Latiice sites on these states can locally go exting (die at some rate/with some probability) or, if empty, be colonized by adjacent occupied sited with a probability proportional to the local occupancy.

For a mathematical description of the process

https://en.wikipedia.org/wiki/Contact_process_(mathematics)

For a guide on the mathematical model for ecological applications, see: Durrett and Levin, 1998

https://royalsocietypublishing.org/doi/10.1098/rstb.1994.0028

doi:

https://doi.org/10.1098/rstb.1994.0028


TO COMPILE:

  We use the Make utility. So, just enter the directory and type make. The makefile is ultra simple for now and it just invocate the compile lines:

    gcc contact.c mt64.c -lm -o contact `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
    gcc contact_2.c mt64.c -lm -o contact_2 `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

respectively (contact, contact_2).


TARGET PROGRAMS:

  The idea is to generate two programs. one (contact) which will do the basics (so it runs in small systems) and another (contact_2) which will have      increasing functionality.

CODE:

  As up today, these two TARGETs are implemented as two different directories (contac_process & contact_process_plus). This produces redundancy, but  also encapsulates simplicity.


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

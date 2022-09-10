# Contact-Process

CONCEPTUAL BACKGROUND:

The Contact process is a general stochastic model for spatial spread. Applications include the growth of bacterial colonies, human cities, tumors, etc.

The process works by spreading (infecting) discrete chunks of space, (sites, locations, hosts, etc) and transforming them from vacant to occupied (discrete state) in continuos time (rates of colonization/birth/infection and extinction/death/recovery). The lattice can also be seen as a population of replicating particles/sites which can die (go extinct locally) and/or, through birth process, colonize near-by vacant locations. It is a model which allows us to study the implications adjacency relationships (access, spatial structure) have in determining the long-term structure of birth-death process. Mathematically the systems corresponds to an Interacting Particle System.

  https://en.wikipedia.org/wiki/Interacting_particle_system
  

SPATIAL STRUCTURE OF THE MODEL (a Lattice):

We conside a 2D lattice (periodic boundary conditions; a torus) representing sp atial locations which can be either vacant (state 0) or occupied (state 1). Latiice sites which are occupied can locally go exting (die at some rate/with some probability) and turn vacant or, if a;ready empty, be colonized by adjacent occupied sited with a probability proportional to the local occupancy.

For a mathematical description of the stochastic process

  https://en.wikipedia.org/wiki/Contact_process_(mathematics)

For a general overview of the mathematical model, see: 

 "Stochastic Spatial Models: A User's Guide to Ecological Applications"
 by Durrett and Levin, 1998. 
 
  published @:

  https://royalsocietypublishing.org/doi/10.1098/rstb.1994.0028

  doi:

  https://doi.org/10.1098/rstb.1994.0028


TARGET PROGRAMS:

  The Repository (Contact-Process) contains two sub-directories (contact_process, contact_process_plus) meant to generate two programs: 
    (i) one (contact) which will do the basics (so it runs in small systems) and
    (ii) another (contact_2) which aims at having increased functionality.


CODE (C language):

  As up today, these two TARGETs are implemented as two different directories containing C code. This produces redundancy (almost all files are repeated on the 2 directories), but  also encapsulates simplicity. The *_plus version has some extra initial conditions choice to start the simulation.


DEPENDENCIES:

1.- [mt64.h]
  Pseudo Random Number Generation (RNG) is achieved using the Mersenne twister algorithm.

    https://en.wikipedia.org/wiki/Mersenne_twister

  The implementation is thanks to the MT19937 algorithm, which for 64 bits, is implemented in the following repository:

    https://github.com/shivakar/rng-reference/tree/master/MT19937

  In particular, here, we use (#include) library mt64.h:

    https://github.com/shivakar/rng-reference/blob/master/MT19937/mt64.h

 2.- [time.h]
  The <time.h> library is used to provide a seed to the RNG 
 
 3.- [math.h]
  Math support for some formulae is provided by the <math.h> library.
 
 4.- [gtk-3]     
  For the GUI Toolkit (Widgets) we use Gtk version 3.

    https://docs.gtk.org/gtk3/
    
  The plan for the near future is to move to gtk-4 and follow the Gnome vision of application development.
  
    https://www.gtk.org/
    
    
TO COMPILE:

  We use the Make utility. So, just enter the proper directory and type:
  
    make 
    
 The makefile is ultra simple for now and it just invocate the following compile lines respectively (contact, contact_2):

    gcc contact.c mt64.c -lm -o contact `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
    gcc contact_2.c mt64.c -lm -o contact_2 `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
 

TESTED PLATFORMS:

The code has been compiled successfully on: 
                                            Ubuntu,
                                            Raspian. 
  

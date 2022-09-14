// LIBS
#include <gtk/gtk.h> 	        // GUI, Gtk Lib
#include "mt64.h"				// Pseudo Random Number Generation 64bit MT Lib	
#include <math.h>				// math to transform random n from continuos to discrete
#include <time.h>				// time library to initialize random generator with time seed

// MACROS						// to later defince a 2D Latice of 256 x 256 sites
#define X_SIZE 256
#define Y_SIZE 256

// STRUCTURE with the simulation DATA
struct simulation
	{
	int lattice_configuration[X_SIZE][Y_SIZE];      // Latice configuration
	gint run; 			        	// time handler tag
	gboolean running;			    // Are we running?
	gboolean initialized;			// Have we been initialized?
	int generation_time;			// generations simulated
	double occupancy;			    // lattice occupancy
	float  dead_rate;			    // particle mortality
	} s;	// instance s of the structure to hold the simulation

// Declare PUT PIXEL function to access individual pixel data on a Pixel Buffer. Implemented at the end of document.
void put_pixel(GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha);

// Pain a background function to make a pixel buffern and an image to display as default canvas
static void paint_a_background (gpointer data)
	{
	GdkPixbuf *p;
	p = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, X_SIZE, Y_SIZE);	
	/*Paint a background canvas for start up image*/
  	int x,y;
	for (x = 0; x < X_SIZE; x++)
	      for (y = 0; y < Y_SIZE; y++)
			put_pixel(p, (int)x, (int)y, (guchar)x, (guchar)y, (guchar)x+y, 255);
	gtk_image_set_from_pixbuf(GTK_IMAGE(data), GDK_PIXBUF(p));
	g_object_unref(p);
	}

// Function to paint lattice DATA from the simulation into a pixbuffer
static void paint_lattice (gpointer data)
	{
   	GdkPixbuf *p;
	p = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, X_SIZE, Y_SIZE);
	// Paint the lattice configuration to a pixbuffer
	int x,y;
	for (x = 0; x < X_SIZE; x++)
		for (y = 0; y < Y_SIZE; y++)
			if(s.lattice_configuration[x][y]==0) 
				// VACANCY is painted WHITE
				{put_pixel(p, (int)x, (int)y, (guchar)255, (guchar)255, (guchar)255, 255);
				}else{ 
				// OCCUPANCY is painted BLACK
			 	put_pixel(p, (int)x, (int)y, (guchar)0, (guchar)0, (guchar)0, 255);
				}
	gtk_image_set_from_pixbuf(GTK_IMAGE(data), GDK_PIXBUF(p));
	g_object_unref(p);
	}

// Up Date Function which simulates the stochastic process and updates the lattice configuration
int update_lattice(gpointer data)
        {
	int random_neighbor_state;
	long random_neighboor;
	int x,y;
	int sites = 0;
	int random_x_coor, random_y_coor;	
	for (sites; sites < (int)(Y_SIZE*X_SIZE); sites++)
		{
		//pick a random focal site
		random_x_coor = (int) floor(genrand64_real3()*X_SIZE); 
		random_y_coor = (int) floor(genrand64_real3()*Y_SIZE);
		switch(s.lattice_configuration[random_x_coor][random_y_coor])
				{
				case 0: // if the site is empty
						// chose a random neighboor from the 4 posible ones
						random_neighboor = (long) floor(genrand64_real3()*4);
						switch(random_neighboor)
									{
									case 0: // south
											random_neighbor_state = s.lattice_configuration[random_x_coor][(int)(((int)Y_SIZE + random_y_coor-1) % (int)Y_SIZE)]; 
											break;
									case 1: // north
											random_neighbor_state =	s.lattice_configuration[random_x_coor][(int)((Y_SIZE + random_y_coor+1) % Y_SIZE)]; 
											break;
									case 2: // east
											random_neighbor_state =	s.lattice_configuration[(int)((X_SIZE + random_x_coor-1)%X_SIZE)][random_y_coor];
											break;
									case 3: // west
											random_neighbor_state =	s.lattice_configuration[(int)((X_SIZE + random_x_coor+1)%X_SIZE)][random_y_coor];
											break;
									}
						// if its random neighbor is occupied: put a 1 copy at the focal site. 
						// Note that when all 4 newighboors are occupied colonization of the focal site happens with probability 1.0.
						if(random_neighbor_state == 1){s.lattice_configuration[random_x_coor][random_y_coor]= 1; s.occupancy ++;}
						break; 
				case 1: // if the site is occupied 
						// if a particle is present at the focal site, it can die with probability dead_rate* dt
						if (genrand64_real2() < s.dead_rate){s.lattice_configuration[random_x_coor][random_y_coor]= 0; s.occupancy --;}
						break;
				}
		}
	s.generation_time ++;
	paint_lattice(data);
	g_print ("\tGeneration:\t%u\t\tOccupancy:\t%f\n", s.generation_time, (s.occupancy/(X_SIZE * Y_SIZE)));
	return 0;
	}

// TIME HANDLER to connect the update function to the Gtk loop for its computation
gboolean time_handler (gpointer data)
	{
	update_lattice(data);
	return TRUE;
	}


// CALL BACK to initialize the lattice button click
static void init_lattice(GtkWidget *widget, gpointer data)
	{
	int r=2;
	//Start with en empty lattice
	int x,y;
	for (x = 0; x < X_SIZE; x++)
                    for (y = 0; y < Y_SIZE; y++) s.lattice_configuration[x][y]=0;
	s.occupancy = 0;
	// then add some r2-size cluster with particles                 
	for (x = 128-r; x < 128+r; x++)
                for (y = 128-r; y < 128+r; y++) 
					{
					s.lattice_configuration[x][y]=1;
					s.occupancy ++;
					}
	s.initialized = TRUE;
	s.generation_time = 0;
	paint_lattice(data);
	g_print ("Lattice initialized\n");
	g_print ("\tGeneration:\t%u\t\tOccupancy:\t%f\n", s.generation_time, (s.occupancy/(X_SIZE * Y_SIZE)));
	}

// CALL BACK to start the simulation button click
static void start_simulation (GtkWidget *widget, gpointer data)
	{
	if(!s.running && s.initialized) 
		{
		s.run = g_idle_add((GSourceFunc) (GSourceFunc) time_handler,GTK_IMAGE(data));  
		s.running = TRUE;
		g_print ("Simulation started\n");
		}
	}
	
// CALL BACK to stop simulation button click
static void stop_simulation (GtkWidget *widget, gpointer   data)
	{
	if(s.running) 
		{
		g_source_remove(s.run);
		s.running = FALSE;
		g_print ("Simulation Stopped\n");
		}
	}

//  CALL BACK to respond Gtk SCALE SLIDE move event
static void mortality_scale_moved (GtkRange *range, gpointer  user_data)
	{
	GtkWidget *label = user_data;
   	gdouble pos = gtk_range_get_value (range);
	s.dead_rate = (float) pos;
	gchar *str = g_strdup_printf ("dead = %.2f", pos);
	gtk_label_set_text (GTK_LABEL (label), str);
	g_free(str);
	}

// HERE GOES THE ABOUT DIALOG BOX For info at a website: lab wiki on the contact process
static void show_about(GtkWidget *widget, gpointer data)
	{
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("kimero_LAB_transparent.tiff", NULL);
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG(dialog),
                                    "Contact Process App");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "v 0.1, 2017");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"Open Source Code");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),
     "The Contact process, a stochastic process, is a model of spatial competition. Particles die and spread in a 2D lattice");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),
     "https://github.com/jekeymer/Contact-Process/wiki");
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
	g_object_unref(pixbuf), pixbuf = NULL;
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
	}

// ACTIVATE function with all Widget Initialization and creation
static void activate (GtkApplication *app, gpointer user_data)
	{
	// declare a bunch of Gtk WIDGETS for the GUI
	GtkWidget *window,*grid, *button, *separator, *image_lattice;
	// to draw into the window images
	GdkPixbuf *pixbuf;

	// to control the parameter of the process
	GtkWidget *mortality_scale, *mortality_label;

	// we initialize the mt algorithm for random number genration
	unsigned long long seed = (unsigned int)time(NULL);
	init_genrand64(seed);

	//define default parameters of the simulation
	s.dead_rate = 0.0; // Richardson tumor growth model
	s.running = FALSE;
	s.initialized = FALSE; // SET SIMULATION FLAGS
		
	/* Create a new WINDOW, and set its title */
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Contact Process");
	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);

	/* Here we make a GRID that is going pack our Widgets */
	grid = gtk_grid_new ();
	/* Pack the GRID  in the window */
	gtk_container_add (GTK_CONTAINER (window), grid);

	// SCALE SLIDE BAR to set and LABEL display  mortality
	mortality_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,1,0.01);
	mortality_label = gtk_label_new ("scale dead rate"); /* LABEL to be shown mortality*/
	g_signal_connect (mortality_scale,"value-changed", G_CALLBACK (mortality_scale_moved), mortality_label);
	gtk_grid_attach (GTK_GRID (grid), mortality_scale, 0, 0, 2, 1); // position (0,0) spanning 2 col and 1 raw
	gtk_grid_attach (GTK_GRID (grid), mortality_label, 2, 0, 3, 1); // position (2,0) spanning 3 col and 1 raw 

	// PIXEL BUFFER @ START UP and LATTICE CONFIGURATION DISPLAY IMAGE
	pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, 0, 8, X_SIZE, Y_SIZE);
	image_lattice = gtk_image_new_from_pixbuf(pixbuf);
	paint_a_background(image_lattice);
	gtk_grid_attach (GTK_GRID (grid), image_lattice, 0, 1, 5, 1); // position (0,1) spanning 5 col and 1 raw) 

	// Separator
	separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_grid_attach (GTK_GRID (grid), separator, 0, 2, 5, 1); // position (0,2) spanning 5 col and 1 raw)

	// -----    INIT BUTTON   -----
	button = gtk_button_new_with_label ("Initialize");
	g_signal_connect (button, "clicked", G_CALLBACK (init_lattice), GTK_IMAGE(image_lattice)); 
	gtk_grid_attach (GTK_GRID (grid), button, 0, 3, 1, 1); // position (0,3) spanning 1 col and 1 raw) 
	// -----   START BUTTON   -----
	button = gtk_button_new_with_label ("Start");
	g_signal_connect (button, "clicked", G_CALLBACK (start_simulation), GTK_IMAGE(image_lattice));
	gtk_grid_attach (GTK_GRID (grid), button, 1, 3, 1, 1); // position (1,3) spanning 1 col and 1 raw)
	// -----  STOP BUTTON    ------
	button = gtk_button_new_with_label ("Stop");
	g_signal_connect (button, "clicked", G_CALLBACK(stop_simulation), NULL);
	gtk_grid_attach (GTK_GRID (grid), button, 2, 3, 1, 1); // position (2,3) spanning 1 col and 1 raw)
	// -----   ABOUT ? BUTTON  ----
	button = gtk_button_new_with_label ("?");
	g_signal_connect (button, "clicked", G_CALLBACK(show_about), GTK_WINDOW(window));
	gtk_grid_attach (GTK_GRID (grid), button, 3, 3, 1, 1); // position (3,3) spanning 1 col and 1 raw)
	//-------   QUIT BUTTON    ----
	button = gtk_button_new_with_label ("Quit"); 
	g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
	gtk_grid_attach (GTK_GRID (grid), button, 4, 3, 1, 1); // position (4,3) spanning 1 col and 1 raw)

	// Show the window and all widgets
	gtk_widget_show_all (window);
	}

// Main 
int main (int    argc, char **argv)
	{
	GtkApplication *app;
	int status;
	app = gtk_application_new ("keymer.lab.contact_process", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);
	return status;
	}

// Implementation of putpixel. Thanks to code from,
// https://developer.gnome.org/gdk-pixbuf/stable/gdk-pixbuf-The-GdkPixbuf-Structure.html
void put_pixel(GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha)
	{
	guchar *pixels, *p;
	int rowstride, numchannels;
	numchannels = gdk_pixbuf_get_n_channels(pixbuf);
	rowstride = gdk_pixbuf_get_rowstride(pixbuf);
	pixels = gdk_pixbuf_get_pixels(pixbuf);
	p = pixels + y * rowstride + x * numchannels;
	p[0] = red;	p[1] = green; p[2] = blue; p[3] = alpha;
	}

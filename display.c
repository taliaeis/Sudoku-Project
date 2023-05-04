#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void destroy () {
    gtk_main_quit();
}

int *getSub(GtkWidget * super_grid,int x,int y) {
	int *arr = malloc(sizeof(int) * 4); 
	int ind = 0;
	int subx = x - (x % 2);//size at 4
	int suby = y - (y % 2);
	for (int j = subx; j < subx+2; j++) {
		for (int k = suby; k < suby+2 ; k++) {
			GtkWidget *label = gtk_grid_get_child_at(super_grid,j,k);
			arr[ind] = atoi(gtk_label_get_text(GTK_LABEL (label)));
			ind++;
		}
	}
}

int main(int argc, char *argv[]) {
	// Widget for the popup window
	GtkWidget *window;
	// Widget for the grid
	GtkWidget *super_grid;
	// Text to display numbers
	GtkWidget *label;
	// Border for the label
	GtkWidget *frame;

	// Default gtk func
	gtk_init(&argc, &argv);
	
	// Create the window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// Title it "Sudoku!"
	gtk_window_set_title(GTK_WINDOW (window), "Sudoku!");
	// Set it to destroy if it receives the signal to destroy
	g_signal_connect(GTK_WINDOW (window), "destroy", G_CALLBACK (destroy), NULL);
	// Give it a 10px padding on the inside so items don't hit the edge
	gtk_container_set_border_width(GTK_CONTAINER (window), 10);

	
	// Create grid
	super_grid = gtk_grid_new();
	// Add the grid to the window
	gtk_container_add(GTK_CONTAINER (window), super_grid);

	// Iterates through rows and columns of the grid
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			// Creates a string to label each cell in the format "i,j"
			char coords[] = {i + '0', ',',  j + '0', '\0'};
			// Create a new label containing the coords
			label = gtk_label_new(coords);
			// Create a new border, pass in NULL so the border itself isn't labelled with anything
			frame = gtk_frame_new(NULL);
			// Make each frame a minimum of 100x100 (pixels?)
			gtk_widget_set_size_request(frame, 100, 100);
			// Add the text into the border
			gtk_container_add((GtkContainer*) frame, label);
			// Add the whole cell into the grid at the correct coords, set the cell to only take up 1 row and 1 column
			gtk_grid_attach((GtkGrid*) super_grid, frame, i, j, 1, 1);
			// Make the text and border visible
			gtk_widget_show(label);
			gtk_widget_show(frame);
		}
	}

	// Make the grid and window visible
	gtk_widget_show(super_grid);
	gtk_widget_show(window);

	gtk_main();

	return 0;
}

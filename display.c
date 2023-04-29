#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

void destroy () {
    gtk_main_quit();
}

//Gets the values in a given column
//Params: GtkGrid *super_grid: the sudoku board; need to typecast to GTK_GRID
//int x: x coordinate of the column to get
//Returns: int[] of values in the column's cells
int *getcol(GtkGrid *super_grid, int x){
	int *arr = malloc(sizeof(int) * 4);
	int size = 4;
	
	for(int i = 0; i < size; i++){
		GtkWidget* frame = gtk_grid_get_child_at(super_grid, x, i);
		GtkWidget* label = gtk_bin_get_child(GTK_BIN (frame));
		arr[i] = atoi(gtk_label_get_text(GTK_LABEL (label)));
	}

	return arr;
}

//Gets the values in a given row
//Params: GtkGrid *super_grid: the sudoku board; need to typecast to GTK_GRID
//int y: y coordinate of the row to get
//Returns: int[] of values in the row's cells
int *getrow(GtkGrid *super_grid, int y){
	int *arr = malloc(sizeof(int) * 4);
	int size = 4;

	for(int i = 0; i < size; i++){
		GtkWidget* frame = gtk_grid_get_child_at(super_grid, i, y);
		GtkWidget* label = gtk_bin_get_child(GTK_BIN (frame));
		arr[i] = atoi(gtk_label_get_text(GTK_LABEL (label)));
	}

	return arr;
}

//Checks for contradictions (for use when inserting numbers)
//Params: GtkGrid *super_grid: the sudoku board; need to typecast to GTK_GRID
//int x, int y: coordinates of the cell to check for contradictions with
//int num: prospective number to be placed at x,y
//Returns: 0 if there are any contradictions; some other int if there are not
int checknum(GtkGrid *super_grid, int x, int y, int num){
	int *arrcol;
	int *arrrow;
	int *arrgrid;
	int size = 0;

	arrcol = getcol(super_grid, x);
	arrrow = getrow(super_grid, y);
	//arrgrid = getsub(*super_grid, x, y);

	size = sizeof(arrcol) / sizeof(int);

	for(int i = 0; i < size; i++){
		if(arrcol[i] == num){
			return 0;
		}

		if(arrrow[i] == num){
			return 0;
		}

		//if(arrgrid[i] == num){
			//return 0;
		//}
	}

	return 1;
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
			char coords[] = {i + '0',  j + '0', '\0'};
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

	// Temporary code to demonstrate getrow and getcol
	g_print("Column 1:\n");
	for (int i = 0; i < 4; i++) {
		g_print("%d\n", getcol(GTK_GRID (super_grid), 1)[i]);
	}

	g_print("Row 3:\n");
	for (int i = 0; i < 4; i++) {
		g_print("%d\n", getrow(GTK_GRID (super_grid), 3)[i]);
	}

	gtk_main();
	
	return 0;
}

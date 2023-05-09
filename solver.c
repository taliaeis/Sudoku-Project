#include <ctype.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

int size;
GtkWidget* super_grid;

void destroy() {
    gtk_main_quit();
}


int *next_empty_cell() {
	int *coor = malloc(sizeof(int) * 2);
	// If no empty cells are left, return coords (-1, -1)
	coor[0] = -1;
	coor[1] = -1;
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			GtkWidget* frame = gtk_grid_get_child_at(GTK_GRID (super_grid), x, y);
			GtkWidget* label = gtk_bin_get_child(GTK_BIN (frame));
			if (0 == (strcmp(gtk_label_get_text(GTK_LABEL (label)), " "))) {
				coor[0] = x;
				coor[1] = y;
				return coor;
			}
		}
	}
	return coor;
}


int *getsub(int x,int y) {
	int *arr = malloc(sizeof(int) * size); 
	int ind = 0;
	int sqrtSize;
	if (size == 9) {
		sqrtSize = 3;
	}
	else {
		sqrtSize = 2;
	}

	int subx = x - (x % sqrtSize);
	int suby = y - (y % sqrtSize);
	for (int j = subx; j < subx + sqrtSize; j++) {
		for (int k = suby; k < suby + sqrtSize; k++) {	
			GtkWidget* frame = gtk_grid_get_child_at(GTK_GRID (super_grid), j, k);
			GtkWidget* label = gtk_bin_get_child(GTK_BIN (frame));	
			arr[ind] = atoi(gtk_label_get_text(GTK_LABEL (label)));
			ind++;

		}
	}
	return arr;
}

//Gets the values in a given column
//int x: x coordinate of the column to get
//Returns: int[] of values in the column's cells
int *getcol(int x){
	int *arr = malloc(sizeof(int) * size);
	
	for(int i = 0; i < size; i++){
		GtkWidget* frame = gtk_grid_get_child_at(GTK_GRID (super_grid), x, i);
		GtkWidget* label = gtk_bin_get_child(GTK_BIN (frame));
		arr[i] = atoi(gtk_label_get_text(GTK_LABEL (label)));
	}

	return arr;
}

//Gets the values in a given row
//int y: y coordinate of the row to get
//Returns: int[] of values in the row's cells
int *getrow(int y){
	int *arr = malloc(sizeof(int) * size);

	for(int i = 0; i < size; i++){
		GtkWidget* frame = gtk_grid_get_child_at(GTK_GRID (super_grid), i, y);
		GtkWidget* label = gtk_bin_get_child(GTK_BIN (frame));
		arr[i] = atoi(gtk_label_get_text(GTK_LABEL (label)));
	}

	return arr;
}

//Checks for contradictions (for use when inserting numbers)
//int x, int y: coordinates of the cell to check for contradictions with
//int num: prospective number to be placed at x,y
//Returns: 0 if there are any contradictions; some other int if there are not
int checknum(int x, int y, int num){
	int *arrcol;
	int *arrrow;
	int *arrsub;
	
	int check = 1;

	arrcol = getcol(x);
	arrrow = getrow(y);
	arrsub = getsub(x, y);


	for(int i = 0; i < size; i++){
		if(arrcol[i] == num){
			check = 0;
		}

		if(arrrow[i] == num){
			check = 0;
		}

		if(arrsub[i] == num){
			check = 0;
		}
	}

	free(arrcol);
	free(arrrow);
	free(arrsub);

	return check;
}

bool test(int *coords){
	int x = coords[0];
	int y = coords[1];	
		
	// Struct used for sleeping during animation, 250000000L represents 1/4th of a second
	struct timespec time = {0, 250000000L};

	free(coords);

	// If coords are negative, all cells are full
	if (x == -1) {
		return true;
	}

	for(int i = 1; i <= size; i++){
		GtkWidget* frame = gtk_grid_get_child_at(GTK_GRID (super_grid), x, y);
		GtkWidget* label = gtk_bin_get_child(GTK_BIN (frame));

		const char *format;
		char *markup;

		if(checknum(x, y, i) == 1){
		char c = i + '0';
			char arr[2] = {c, '\0'};

			// Set label text and font size
			format = "<span font=\"36\">%s</span>";
			markup = g_markup_printf_escaped(format, arr);

			gtk_label_set_markup(GTK_LABEL (label), markup);
			nanosleep(&time, NULL);
			while(gtk_events_pending()) {
				gtk_main_iteration();
			}

			g_free(markup);

			//tests next cell
			bool next = test(next_empty_cell());
			

			if(next == false){
				format = "<span foreground=\"red\" font=\"36\">%s</span>";
				markup = g_markup_printf_escaped(format, gtk_label_get_text(GTK_LABEL (label)));
				gtk_label_set_markup(GTK_LABEL (label), markup);
			
				nanosleep(&time, NULL);
				while(gtk_events_pending()) {
					gtk_main_iteration();
				}
				
				// Clear current label
				gtk_label_set_text(GTK_LABEL (label), " ");
				nanosleep(&time, NULL);
				while(gtk_events_pending()) {
					gtk_main_iteration();
				}
				continue;
			}
			if(next == true){
				return true;
			}
		}
	}
	return false;
}

void init_grid(char *filename) {

	GtkWidget *label;
	GtkWidget *frame;
	FILE *fp = fopen(filename, "r");

	char c = fgetc(fp);

	if (c == EOF) {
		fprintf(stderr, "Error: Please enter a non-empty Sudoku starter file\n");
		exit(1);
	}

	// Set the side length size of the Sudoku
	size = c - '0';

	if (size != 4 && size != 9) {
		fprintf(stderr, "Error: Invalid Sudoku size, please choose between 4 or 9\n");
		exit(1);
	}

	// Create grid
	super_grid = gtk_grid_new();

	// Populate grid with empty cells
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			
			label = gtk_label_new(" ");
			
			// Create a new border, pass in NULL so the border itself isn't labelled with anything
			frame = gtk_frame_new(NULL);
			// Make each frame a minimum of 100x100 (pixels?)
			gtk_widget_set_size_request(frame, 100, 100);
			// Add the text into the border
			gtk_container_add((GtkContainer*) frame, label);
			// Add the whole cell into the grid at the correct coords, set the cell to only take up 1 row and 1 column
			gtk_grid_attach((GtkGrid*) super_grid, frame, x, y, 1, 1);
			// Make the text and border visible
			gtk_widget_show(label);
			gtk_widget_show(frame);
		}
	}

	// Fill cells with values
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			c = fgetc(fp);
			if (c == EOF || c == '\n') {
				fprintf(stderr, "Error: Incorrect number of values in starter file for Sudoku of size %d\n", size);
				exit(1);
			}

			// - represents a blank cell, all other values other than digits are invalid
			if (!isdigit(c)) {
				if (c != '-') {
					fprintf(stderr, "Error: Invalid char in starter file: '%c' \n", c);
					exit(1);
				}
				continue;
			}
			
			// Check for invalid digits
			if (c - '0' > (size * size) || c - '0' == 0) {
				fprintf(stderr, "Error: Invalid digit %c in starter file for Sudoku of size %d\n", c, size);
				exit(1);
			}

			// Store text for label
			char text[] = {c, '\0'};
			// Check for contradictions in starting config
			if (checknum(x, y, c - '0') != 1) {
				fprintf(stderr, "Error: Contradiction in starter file at position (%d, %d)\n", x, y);
				exit(1);
			}

			// Update label text to starter value
			frame = gtk_grid_get_child_at(GTK_GRID (super_grid), x, y);
			label = gtk_bin_get_child(GTK_BIN (frame));
			//gtk_label_set_text(GTK_LABEL (label), text);

			// Make label bold
			const char *format = "<span font=\"36\"><b>%s</b></span>";
			char *markup = g_markup_printf_escaped(format, text);

			gtk_label_set_markup(GTK_LABEL (label), markup);
			g_free(markup);
		}
	}

	fclose(fp);
}

int main(int argc, char *argv[]) {
	// Widget for the popup window
	GtkWidget *window;

	// Default gtk func
	gtk_init(&argc, &argv);
	
	if (argc < 2) {
		fprintf(stderr, "Error: Please enter a Sudoku starter file\n");
		exit(1);
	}

	// Initiate the grid with the first argument, a path to a file representation of the board
	init_grid(argv[1]);

	
	
	// Create the window
	GtkWidget *btn_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// Title it "Solve!"
	gtk_window_set_title(GTK_WINDOW (btn_window), "Solve!");
	// Set it to destroy if it receives the signal to destroy
	g_signal_connect(GTK_WINDOW (btn_window), "destroy", G_CALLBACK (destroy), NULL);
	// Give it a 10px padding on the inside so items don't hit the edge
	gtk_container_set_border_width(GTK_CONTAINER (btn_window), 10);

	
	// Create the window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// Title it "Sudoku!"
	gtk_window_set_title(GTK_WINDOW (window), "Sudoku!");
	// Set it to destroy if it receives the signal to destroy
	g_signal_connect(GTK_WINDOW (window), "destroy", G_CALLBACK (destroy), NULL);
	// Give it a 10px padding on the inside so items don't hit the edge
	gtk_container_set_border_width(GTK_CONTAINER (window), 10);
	
	
	//make button to initiate solving
	GtkWidget *button = gtk_button_new_with_label("Solve");
	
	
	// Add the grid to the window
	gtk_container_add(GTK_CONTAINER (window), super_grid);
	gtk_container_add(GTK_CONTAINER (btn_window), button);	
	// Show grid and window and button
	gtk_widget_show(super_grid);	
	gtk_widget_show(button);
	gtk_widget_show(window);
	gtk_widget_show(btn_window);

	//if the button is clicked, solve the sudoku
	g_signal_connect(button, "clicked", G_CALLBACK(test(next_empty_cell())), NULL);

	gtk_main();
	
	return 0;
	}

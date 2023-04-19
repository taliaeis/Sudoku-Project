#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

void destroy () {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *label;

	char digits[9][10] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	grid = gtk_grid_new();

	gtk_window_set_title(GTK_WINDOW(window), "Sudoku!");

	g_signal_connect(GTK_WINDOW(window), "destroy", G_CALLBACK(destroy), NULL);

	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	gtk_container_add(GTK_CONTAINER(window), grid);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			char *temp_label = digits[i];
			strcat(temp_label, " ,");
			strcat(temp_label, digits[j]);
			label = gtk_label_new(temp_label);
			gtk_grid_attach((GtkGrid*) grid, label, i, j, 1, 1);
			gtk_widget_show(label);
		}
	}

	gtk_widget_show(grid);


	gtk_widget_show(window);

	gtk_main();

	return 0;
}

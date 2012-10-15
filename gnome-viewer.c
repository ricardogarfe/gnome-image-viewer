#include <gtk/gtk.h>

static void on_open_image(GtkButton* button, gpointer user_data)
{
	GtkWidget *image = GTK_WIDGET (user_data);
	GtkWidget *toplevel = gtk_widget_get_toplevel (image);
	GtkFileFilter *filter = gtk_file_filter_new ();
	GtkWidget *dialog = gtk_file_chooser_dialog_new (("Open image"),
	                                                 GTK_WINDOW (toplevel),
	                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
	                                                 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                                 NULL);

	gtk_file_filter_add_pixbuf_formats (filter);
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),
	                             filter);

	switch (gtk_dialog_run (GTK_DIALOG (dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
		{
			gchar *filename =
				gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
			gtk_image_set_from_file (GTK_IMAGE (image), filename);
			break;
		}
		default:
			break;
	}
	gtk_widget_destroy (dialog);
}

static GtkWidget* create_window (void)
{
	GtkWidget *window;
    GtkScrolledWindow *scrolledWindow;
	GtkWidget *button;
	GtkWidget *image;
    GdkPixbuf *imageBuf;
    GError *error = NULL;
	GtkWidget *box;

    int width, height;

	// Window.
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "image-viewer-c");

	// Signal to Exit when the window is closed.
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_widget_show(window);

    // Container Box.
	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);

    // Add to the container.
    gtk_container_add (GTK_CONTAINER (window), box);

    // Scrolled Window to include image.
    scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

    gtk_widget_show(scrolledWindow);

    // Button.
	button = gtk_button_new_with_label (("Open image"));
    gtk_widget_show(button);

    // Image Buffer.
    imageBuf = gdk_pixbuf_new_from_file ("images/doors.jpg", &error);
    width = gdk_pixbuf_get_width (imageBuf);
    height = gdk_pixbuf_get_height (imageBuf);

    // Image.
    image = gtk_image_new_from_file("images/doors.jpg");

    // Default size for the window. 
    gtk_widget_set_size_request (window, width<500?width:500,
                               height<500?height:500);
    gtk_widget_show(image);

    // Use ViewPort to set scrollWidow to image.
    gtk_scrolled_window_add_with_viewport(scrolledWindow, image);

    // Add Widgets.
    gtk_box_pack_start (GTK_BOX (box), scrolledWindow, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (box), image, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (box), button, FALSE, FALSE, 0);

	// Show open dialog when opening a file.
	g_signal_connect (button, "clicked", G_CALLBACK (on_open_image), image);

	return window;
}

int main (int argc, char **argv)
{

    // Initialize gtk.
    gtk_init(&argc, &argv);

    GtkWidget *window;
    window = create_window();

    gtk_widget_show_all(GTK_WIDGET (window));

    gtk_main();

    return 0;
}


/**
* @brief GTK Functions
* @file s9xml.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 27/07/2012
*
*/


#ifndef S9_GTK_FUNCTIONS_HPP
#define S9_GTK_FUNCTIONS_HPP

#include <gtkmm.h>
#include "../common.hpp"

inline std::string loadFileDialog() {

	Gtk::FileChooserDialog dialog("Please choose a File", Gtk::FILE_CHOOSER_ACTION_OPEN);

	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);
	
	/*Glib::RefPtr<Gtk::FileFilter> filter_text = Gtk::FileFilter::create();
	filter_text->set_name("PCD Files");
	filter_text->add_mime_type("text/plain");
	dialog.add_filter(filter_text);*/

	int result = dialog.run();

	//Handle the response:
	switch(result) {
		case(Gtk::RESPONSE_OK): {		
			return dialog.get_filename();
			break;
		}
		case(Gtk::RESPONSE_CANCEL): {
			return std::string();
			break;
		}
		default:{
			return std::string();
			break;
		}
	}
	return std::string();
}

#endif
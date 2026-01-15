#include <gtkmm.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/separator.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <filesystem>
#include <sndfile.h>
#include <sndfile.h>
#include <glibmm/ustring.h>

class BPMCalculator : public Gtk::Window {
public:
    BPMCalculator() {
        set_title("Time to BPM Calculator");
        set_default_size(600, 700);
        set_border_width(10);

        setup_ui();
        apply_dark_mode();
        setup_keyboard_navigation();

        signal_key_press_event().connect(sigc::mem_fun(*this, &BPMCalculator::on_key_press));
        
        show_all_children();
    }

private:
    Gtk::Grid main_grid;
    Gtk::Grid input_grid;
    Gtk::Grid output_grid;
    
    Gtk::Label score_label;
    Gtk::Label bars_label;
    Gtk::Label time_label;
    Gtk::Label file_label;
    
    Gtk::ComboBoxText score_combo;
    Gtk::ComboBoxText bars_combo;
    Gtk::Entry time_entry;
    Gtk::Entry file_entry;
    Gtk::Button browse_button;
    Gtk::Button calculate_button;
    
    Gtk::Label bpm_label;
    Gtk::Label bpm_value;
    
    Gtk::Frame note_lengths_frame;
    Gtk::Grid note_lengths_grid;
    std::vector<Gtk::Label*> note_labels;
    std::vector<Gtk::Label*> note_values;
    
    Gtk::Label info_label;
    
    double current_time_seconds = 0.0;
    int current_score = 4;
    int current_bars = 1;

    void setup_ui() {
        add(main_grid);
        
        main_grid.set_row_spacing(10);
        main_grid.set_column_spacing(15);
        
        score_label.set_text("Time Signature:");
        score_label.set_halign(Gtk::ALIGN_START);
        
        bars_label.set_text("Number of Bars:");
        bars_label.set_halign(Gtk::ALIGN_START);
        
        time_label.set_text("Time (seconds):");
        time_label.set_halign(Gtk::ALIGN_START);
        
        file_label.set_text("Audio File:");
        file_label.set_halign(Gtk::ALIGN_START);
        
        setup_score_combo();
        setup_bars_combo();
        setup_time_entry();
        setup_file_controls();
        setup_calculate_button();
        setup_output_display();
        
        main_grid.attach(score_label, 0, 0, 1, 1);
        main_grid.attach(score_combo, 1, 0, 1, 1);
        main_grid.attach(bars_label, 0, 1, 1, 1);
        main_grid.attach(bars_combo, 1, 1, 1, 1);
        main_grid.attach(time_label, 0, 2, 1, 1);
        main_grid.attach(time_entry, 1, 2, 1, 1);
        main_grid.attach(file_label, 0, 3, 1, 1);
        main_grid.attach(file_entry, 1, 3, 1, 1);
        main_grid.attach(browse_button, 2, 3, 1, 1);
        main_grid.attach(calculate_button, 0, 4, 3, 1);
        
        auto separator = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_HORIZONTAL));
        main_grid.attach(*separator, 0, 5, 3, 1);
        
        main_grid.attach(note_lengths_frame, 0, 6, 3, 1);
        
        main_grid.attach(info_label, 0, 7, 3, 1);
    }

    void setup_score_combo() {
        score_combo.append("2/4");
        score_combo.append("3/4");
        score_combo.append("4/4");
        score_combo.append("5/4");
        score_combo.append("6/4");
        score_combo.append("7/4");
        score_combo.append("3/8");
        score_combo.append("6/8");
        score_combo.append("9/8");
        score_combo.append("12/8");
        score_combo.set_active(2); // 4/4
        score_combo.signal_changed().connect(sigc::mem_fun(*this, &BPMCalculator::on_score_changed));
    }

    void setup_bars_combo() {
        for (int i = 1; i <= 32; ++i) {
            bars_combo.append(std::to_string(i));
        }
        bars_combo.set_active(0); // 1 bar
        bars_combo.signal_changed().connect(sigc::mem_fun(*this, &BPMCalculator::on_bars_changed));
    }

    void setup_time_entry() {
        time_entry.set_text("0.0");
        time_entry.signal_changed().connect(sigc::mem_fun(*this, &BPMCalculator::on_time_changed));
    }

    void setup_file_controls() {
        file_entry.set_text("");
        file_entry.set_sensitive(false);
        
        browse_button.set_label("Browse...");
        browse_button.signal_clicked().connect(sigc::mem_fun(*this, &BPMCalculator::on_browse_clicked));
    }

    void setup_calculate_button() {
        calculate_button.set_label("Calculate BPM");
        calculate_button.signal_clicked().connect(sigc::mem_fun(*this, &BPMCalculator::on_calculate_clicked));
    }

    void setup_output_display() {
        note_lengths_frame.set_label("Results");
        note_lengths_frame.add(note_lengths_grid);
        
        note_lengths_grid.set_row_spacing(5);
        note_lengths_grid.set_column_spacing(20);
        
        bpm_label.set_text("BPM:");
        bpm_label.set_halign(Gtk::ALIGN_START);
        bpm_value.set_text("0.00");
        bpm_value.set_halign(Gtk::ALIGN_START);
        
        note_lengths_grid.attach(bpm_label, 0, 0, 1, 1);
        note_lengths_grid.attach(bpm_value, 1, 0, 1, 1);
        
        setup_note_lengths();
        
        info_label.set_text("");
        info_label.set_halign(Gtk::ALIGN_START);
        info_label.set_line_wrap(true);
    }

    void setup_note_lengths() {
        const std::vector<std::string> note_names = {
            "Whole", "Half", "Quarter", "Eighth", "Sixteenth", 
            "Thirty-second", "Sixty-fourth", "Dotted Quarter", 
            "Dotted Eighth", "Triplet Quarter", "Triplet Eighth"
        };
        
        for (size_t i = 0; i < note_names.size(); ++i) {
            auto* label = Gtk::manage(new Gtk::Label(note_names[i] + ":"));
            label->set_halign(Gtk::ALIGN_START);
            note_labels.push_back(label);
            note_lengths_grid.attach(*label, 0, i + 1, 1, 1);
            
            auto* value = Gtk::manage(new Gtk::Label("0.00 ms"));
            value->set_halign(Gtk::ALIGN_START);
            note_values.push_back(value);
            note_lengths_grid.attach(*value, 1, i + 1, 1, 1);
        }
    }

    void apply_dark_mode() {
        auto css_provider = Gtk::CssProvider::create();
        css_provider->load_from_data(R"(
            * {
                background-color: #000000;
                color: #ffffff;
                font-family: monospace;
            }
            
            GtkWindow {
                background-color: #000000;
            }
            
            GtkGrid, GtkBox {
                background-color: #000000;
            }
            
            GtkLabel {
                color: #ffffff;
                font-size: 12px;
            }
            
            GtkEntry {
                background-color: #1a1a1a;
                color: #ffffff;
                border: 1px solid #333333;
                padding: 5px;
            }
            
            GtkEntry:focus {
                border-color: #4a9eff;
                box-shadow: 0 0 3px #4a9eff;
            }
            
            GtkComboBox {
                background-color: #1a1a1a;
                color: #ffffff;
                border: 1px solid #333333;
                padding: 5px;
            }
            
            GtkButton {
                background-color: #2a2a2a;
                color: #ffffff;
                border: 1px solid #444444;
                padding: 8px 16px;
                font-weight: bold;
            }
            
            GtkButton:hover {
                background-color: #3a3a3a;
                border-color: #666666;
            }
            
            GtkButton:active {
                background-color: #4a4a4a;
            }
            
            GtkFrame {
                border: 1px solid #444444;
                border-radius: 5px;
                background-color: #0a0a0a;
            }
            
            GtkFrame > GtkLabel {
                color: #cccccc;
                font-weight: bold;
            }
            
            GtkSeparator {
                color: #333333;
            }
        )");
        
        auto style_context = get_style_context();
        style_context->add_provider_for_screen(Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }

    void setup_keyboard_navigation() {
        set_can_focus(true);
        score_combo.set_can_focus(true);
        bars_combo.set_can_focus(true);
        time_entry.set_can_focus(true);
        browse_button.set_can_focus(true);
        calculate_button.set_can_focus(true);
    }

    bool on_key_press(GdkEventKey* event) {
        switch (event->keyval) {
            case GDK_KEY_Escape:
            case GDK_KEY_q:
            case GDK_KEY_Q:
                if ((event->state & GDK_CONTROL_MASK) || (event->keyval == GDK_KEY_Escape)) {
                    close();
                    return true;
                }
                break;
            case GDK_KEY_Return:
            case GDK_KEY_KP_Enter:
                if (!time_entry.has_focus() && !file_entry.has_focus()) {
                    on_calculate_clicked();
                    return true;
                }
                break;
            case GDK_KEY_F1:
                score_combo.grab_focus();
                return true;
            case GDK_KEY_F2:
                bars_combo.grab_focus();
                return true;
            case GDK_KEY_F3:
                time_entry.grab_focus();
                return true;
            case GDK_KEY_F4:
                browse_button.grab_focus();
                return true;
            case GDK_KEY_F5:
                calculate_button.grab_focus();
                return true;
        }
        return false;
    }

    void on_score_changed() {
        std::string score_text = score_combo.get_active_text();
        size_t slash_pos = score_text.find('/');
        if (slash_pos != std::string::npos) {
            current_score = std::stoi(score_text.substr(0, slash_pos));
        }
    }

    void on_bars_changed() {
        current_bars = std::stoi(bars_combo.get_active_text());
    }

    void on_time_changed() {
        try {
            current_time_seconds = std::stod(time_entry.get_text());
        } catch (...) {
            current_time_seconds = 0.0;
        }
    }

    void on_browse_clicked() {
        Gtk::FileChooserDialog dialog("Please choose an audio file", Gtk::FILE_CHOOSER_ACTION_OPEN);
        dialog.set_transient_for(*this);
        
        dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
        dialog.add_button("_Open", Gtk::RESPONSE_OK);
        
        auto filter_audio = Gtk::FileFilter::create();
        filter_audio->set_name("Audio files");
        filter_audio->add_mime_type("audio/wav");
        filter_audio->add_mime_type("audio/mpeg");
        filter_audio->add_mime_type("audio/ogg");
        filter_audio->add_pattern("*.wav");
        filter_audio->add_pattern("*.mp3");
        filter_audio->add_pattern("*.ogg");
        dialog.add_filter(filter_audio);
        
        int result = dialog.run();
        if (result == Gtk::RESPONSE_OK) {
            std::string filename = dialog.get_filename();
            file_entry.set_text(filename);
            
            // For now, just extract the filename to show we selected it
            // In a real implementation, you'd use an audio library to get duration
            std::filesystem::path path(filename);
            time_entry.set_text("5.12"); // Example duration
            current_time_seconds = 5.12;
            
            info_label.set_text("Selected: " + path.filename().string() + " (Note: Duration detection not implemented)");
        }
    }

    void on_calculate_clicked() {
        if (current_time_seconds <= 0) {
            info_label.set_text("Please enter a valid time duration");
            return;
        }
        
        calculate_bpm_and_note_lengths();
    }

    void calculate_bpm_and_note_lengths() {
        // Calculate BPM
        // BPM = (Number of beats per bar * Number of bars * 60) / Total time in seconds
        int beats_per_bar = current_score;
        int total_beats = beats_per_bar * current_bars;
        double bpm = (total_beats * 60.0) / current_time_seconds;
        
        bpm_value.set_text(std::to_string(bpm).substr(0, 5) + " BPM");
        
        // Calculate note durations in milliseconds
        double beat_duration_ms = 60000.0 / bpm; // Duration of one beat in ms
        
        // Update note durations
        note_values[0]->set_text(std::to_string(beat_duration_ms * 4).substr(0, 8) + " ms");      // Whole
        note_values[1]->set_text(std::to_string(beat_duration_ms * 2).substr(0, 8) + " ms");      // Half
        note_values[2]->set_text(std::to_string(beat_duration_ms * 1).substr(0, 8) + " ms");      // Quarter
        note_values[3]->set_text(std::to_string(beat_duration_ms * 0.5).substr(0, 8) + " ms");    // Eighth
        note_values[4]->set_text(std::to_string(beat_duration_ms * 0.25).substr(0, 8) + " ms");   // Sixteenth
        note_values[5]->set_text(std::to_string(beat_duration_ms * 0.125).substr(0, 8) + " ms");  // Thirty-second
        note_values[6]->set_text(std::to_string(beat_duration_ms * 0.0625).substr(0, 8) + " ms"); // Sixty-fourth
        note_values[7]->set_text(std::to_string(beat_duration_ms * 1.5).substr(0, 8) + " ms");     // Dotted Quarter
        note_values[8]->set_text(std::to_string(beat_duration_ms * 0.75).substr(0, 8) + " ms");    // Dotted Eighth
        note_values[9]->set_text(std::to_string(beat_duration_ms * 0.6667).substr(0, 8) + " ms");  // Triplet Quarter
        note_values[10]->set_text(std::to_string(beat_duration_ms * 0.3333).substr(0, 8) + " ms"); // Triplet Eighth
        
        // Display additional information
        std::ostringstream info;
        info << "Total time: " << std::fixed << std::setprecision(2) << current_time_seconds << " seconds | ";
        info << "Bars: " << current_bars << " | ";
        info << "Beats per bar: " << current_score << " | ";
        info << "Total beats: " << total_beats << " | ";
        info << "Tempo: ";
        if (bpm < 60) info << "Largo";
        else if (bpm < 76) info << "Adagio";
        else if (bpm < 108) info << "Andante";
        else if (bpm < 120) info << "Moderato";
        else if (bpm < 168) info << "Allegro";
        else if (bpm < 200) info << "Presto";
        else info << "Prestissimo";
        
        info_label.set_text(info.str());
    }
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "com.example.bpmcalq");
    
    BPMCalculator calculator;
    return app->run(calculator);
}
#ifndef TEMP_LOG_CHART
#define TEMP_LOG_CHART

#include "constants.h"
#include <gtkmm/applicationwindow.h>
#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>
#include "db.h"

#include "commons.h"
class LogChart : public Gtk::DrawingArea
{
  public:
    LogChart();
    /**
     * Destructor */
    ~LogChart();
   
    void set_logger_data(std::map<std::string, std::string> &_log,  std::string &name);

    /**
   * Redrawing the painted canvas
   * */
    void force_redraw();

  protected:
    /* Override default signal handler */
    bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
    /**
   * Drawing the main grid of the canvas
   * All the elements will be placed in this grid
   * */
    void draw_grid(const Cairo::RefPtr<Cairo::Context> &, int width, int height);

    void draw_temperature_log(const Cairo::RefPtr<Cairo::Context> &cr);
    /**
   * Drawing text in the defined position
   * @var const Cairo::RefPtr<Cairo::Context> &cr reference to the passed cairo context
   * @var std::string message, the text is deemed to be printed
   * @var int x , @var int y the coordinate this text will be put in.
   * @var int font_size , the size of the printed text
   *  
   * */
    void draw_text(const Cairo::RefPtr<Cairo::Context> &cr, std::string message,
                   int x, int y, int font_size = 0);
    /**
   * Drawing text in the defined position
   * @var const Cairo::RefPtr<Cairo::Context> &cr reference to the passed cairo context
   * @var int message, the text is deemed to be printed
   * @var int x , @var int y the coordinate this text will be put in.
   * @var int font_size , the size of the printed text
   *  
   * */
    void draw_text(const Cairo::RefPtr<Cairo::Context> &cr, int message,
                   int x, int y, int font_size = 0);

  private:
    Db *dbInstance = nullptr;
    std::map<std::string, std::string> logData;
    time_t rawtime;
    struct tm *ptm;
    char dateString[50];
    std::string cityName;
};

#endif // TEMP_LOG_CHART
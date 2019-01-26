#include "log_chart.h"

LogChart::LogChart() 
{
    set_size_request(CANVAS_WIDTH, CANVAS_HEIGHT);

    dbInstance = Db::getInstance();
}

LogChart::~LogChart()
{
}

bool LogChart::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    draw_grid(cr, width, height);
    return true;
}

void LogChart::draw_grid(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height)
{
   
    cr->set_line_width(.1);
    cr->set_source_rgba(.5, .4, .6, .7);
    cr->fill();

    for (int i = -CANVAS_HEIGHT / (2*scale); i < CANVAS_HEIGHT / (2*scale); i += 10)
    {
        cr->move_to(10, CANVAS_HEIGHT / 2 + scale*i );
        cr->line_to(CANVAS_WIDTH, CANVAS_HEIGHT / 2 + scale*i);
        cr->stroke_preserve();
        draw_text(cr, std::to_string(-i) + Constants::getInstance()->get_selected_temperature_symbol(),
                  CANVAS_WIDTH - 15,
                  CANVAS_HEIGHT / 2 + scale*i - 5,
                  9);
    }

    draw_temperature_log(cr);
}
void LogChart::draw_temperature_log(const Cairo::RefPtr<Cairo::Context> &cr)
{

    int temp, startOffsetX, windowWidth = 0, _time, lastPrintedDateX = 0;
    if (logData.size() == 0)
        return;
    cr->set_source_rgba(.9, .4, .2, 1);
    cr->fill();
    draw_text(cr, cityName, CANVAS_WIDTH / 2, 30, 20);
    Point startPoint, endPoint;

    Constants::getInstance()->convert_temperature(logData.begin()->second, temp, KELVIN);

    startOffsetX = std::stoi(logData.begin()->first);
    windowWidth = std::stoi(logData.rbegin()->first) - startOffsetX;
    int step = 0;

    cr->set_line_width(2);
    cr->set_source_rgba(.9, .4, .2, 1);
    cr->fill();

    for (auto const &it : logData)
    {
        _time = std::stoi(it.first);
        rawtime = (time_t)_time;
        ptm = localtime(&rawtime);
        endPoint.x = (_time - startOffsetX) * ((CANVAS_WIDTH - 50) / (double)(windowWidth));
        endPoint.y = CANVAS_HEIGHT / 2 - scale*(temp);
        if (step > 1 && (endPoint.x - lastPrintedDateX) > 50)
        {
            strftime(dateString, 50, "%d-%m-%y", ptm);
            draw_text(cr, dateString, endPoint.x, CANVAS_HEIGHT - 30, 10);
            lastPrintedDateX = endPoint.x;
        }

        Constants::getInstance()->convert_temperature(it.second, temp, KELVIN);
        if (step > 0)
        {
            cr->move_to(startPoint.x, startPoint.y);
            cr->line_to(endPoint.x, endPoint.y);
            cr->stroke_preserve();
        }
        startPoint = endPoint;
        step++;
    }
}
void LogChart::draw_text(const Cairo::RefPtr<Cairo::Context> &cr, std::string message, int x, int y, int font_size)
{
    Pango::FontDescription font;

    font.set_family("Monospace");
    font.set_weight(Pango::WEIGHT_BOLD);

    if (font_size != 0)
        font.set_absolute_size(font_size * Pango::SCALE);

    auto layout = create_pango_layout(Glib::ustring(message));

    layout->set_font_description(font);

    int textWidth;
    int textHeight;
    layout->get_pixel_size(textWidth, textHeight);
    cr->move_to((x - textWidth / 2), (y - textHeight / 2));
    layout->show_in_cairo_context(cr);
}

void LogChart::set_logger_data(std::map<std::string, std::string> &_log, std::string &name , const int &s)
{
    logData = _log;
    cityName = name;
    scale = s;
    force_redraw();
}

// force the redraw of the image
void LogChart::force_redraw()
{
    auto win = get_window();
    if (win)
    {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
}

void LogChart::draw_text(const Cairo::RefPtr<Cairo::Context> &cr, int message, int x, int y, int font_size)
{
    draw_text(cr, std::to_string(message), x, y, font_size);
}

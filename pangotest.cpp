#include <iostream>
#include <string>

#include <cairo.h>
#include <pango/pangocairo.h>


static const char unicodeTestString[] =
    "ü Ұ Љ ټ Ω";

void
renderText(
  const std::string& text, const std::string& fontname, unsigned int fontsize,
  unsigned int width, unsigned int height,
  const std::string& pngName, const std::string& jsonName
)
{
  std::cerr << "-- renderText begin " << pngName << " | " << jsonName << std::endl;

  // create context
  PangoContext* context = pango_context_new();
  PangoFontMap* fontmap = pango_cairo_font_map_get_default();
  pango_context_set_font_map(context, fontmap);

  cairo_font_options_t* options = cairo_font_options_create();
  cairo_font_options_set_antialias(options, CAIRO_ANTIALIAS_NONE);
  cairo_font_options_set_hint_style(options, CAIRO_HINT_STYLE_FULL);
  cairo_font_options_set_hint_metrics(options, CAIRO_HINT_METRICS_ON);
  cairo_font_options_set_subpixel_order(options, CAIRO_SUBPIXEL_ORDER_BGR);
  pango_cairo_context_set_font_options(context, options);

  // set font description
  PangoFontDescription* desc = pango_font_description_new();
  pango_font_description_set_family(desc, fontname.c_str() );
  pango_font_description_set_size(desc, fontsize * PANGO_SCALE );

  // create layout
  PangoLayout* layout = pango_layout_new(context);

  // render the complete string without wrapping
  pango_layout_set_text(layout, text.c_str(), -1);
  pango_layout_set_font_description(layout, desc);
  pango_layout_set_width(layout, -1);

  cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t* cr = cairo_create(surface);

  // paint background
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // white
  cairo_paint(cr);
  // print text
  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // black
  pango_cairo_show_layout(cr, layout);

  if(CAIRO_STATUS_SUCCESS != cairo_surface_write_to_png(surface, pngName.c_str())) {
    std::cerr <<  "cannot write " + pngName << std::endl;
    exit(1);
  }

  if(!pango_layout_write_to_file(layout, (PangoLayoutSerializeFlags)(PANGO_LAYOUT_SERIALIZE_CONTEXT | PANGO_LAYOUT_SERIALIZE_OUTPUT), jsonName.c_str(), NULL)) {
    std::cerr << "cannot write " + jsonName << std::endl;
    exit(1);
  }

  cairo_destroy(cr);
  cairo_surface_destroy(surface);

  g_object_unref(layout);

  pango_font_description_free(desc);

  cairo_font_options_destroy(options);

  g_object_unref(context);

  std::cerr << "-- renderText end " << pngName << " | " << jsonName << std::endl;
}

void pngCompare(
  const std::string& pngName, const std::string& refName
) {
  std::cerr << "-- pngCompare begin " << pngName << " | " << refName << std::endl;

  cairo_surface_t* surface1 = cairo_image_surface_create_from_png(pngName.c_str());
  if( !surface1 ) {
    std::cerr <<  "cannot read " + pngName << std::endl;
    exit(1);
  }
  cairo_surface_flush(surface1);

  cairo_surface_t* surface2 = cairo_image_surface_create_from_png(refName.c_str());
  if( !surface2 ) {
    std::cerr <<  "cannot read " + refName << std::endl;
    exit(1);
  }
  cairo_surface_flush(surface2);

  bool fok = true;

  int w1 = cairo_image_surface_get_width(surface1);
  int h1 = cairo_image_surface_get_height(surface1);
  int s1 = cairo_image_surface_get_stride(surface1);

  int w2 = cairo_image_surface_get_width(surface2);
  int h2 = cairo_image_surface_get_height(surface2);
  int s2 = cairo_image_surface_get_stride(surface2);

  if( w1 != w2 ) {
    std::cerr << "different width " << pngName << " | " << refName << std::endl;
    exit(1);
  }

  if( h1 != h2 ) {
    std::cerr << "different height " << pngName << " | " << refName << std::endl;
    exit(1);
  }

  const unsigned char* data1 = cairo_image_surface_get_data(surface1);
  const unsigned char* data2 = cairo_image_surface_get_data(surface2);

  for( unsigned int y = 0; y < h1; y++ ) {
    unsigned int srcofs = y*s1;
    unsigned int dstofs = y*s2;
    for( unsigned int x = 0; x < w1; x++ ) {
      for( unsigned int c = 0; c < 4; c++ ) {
        if( data2[dstofs++] != data1[srcofs++] ) {
          std::cerr << "different pixel " << pngName << " | " << refName << " (" << x << "," << y << ")" << std::endl;
          exit(1);
        }
      }
    }
  }

  cairo_surface_destroy(surface2);
  cairo_surface_destroy(surface1);

  std::cerr << "-- pngCompare end " << pngName << " | " << refName << std::endl;
}

int main( int argc, char* argv[] )
{
    if( argc != 4 ) {
	std::cerr << "syntax: ./a.out tag1 tag2 dir" << std::endl;
	exit(1);
    }

    std::string text = unicodeTestString;
    std::string font("Roboto");
    for( int k = 8; k <= 32; k++ ) {
        std::string dir = argv[3];
        std::string dumpName = "textchunk.dump";
        std::string pngName = dir + "/" + dumpName + "." + argv[1] + "." + std::to_string(k) + ".png";
        std::string refName = dir + "/" + dumpName + "." + argv[2] + "." + std::to_string(k) + ".png";
        std::string jsonName = dir + "/" + dumpName + "." + argv[1] + "." + std::to_string(k) + ".json";
        renderText(text, font, k, 400, 100, pngName, jsonName);
        pngCompare(pngName, refName);
    }

    return 0;
}


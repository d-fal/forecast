CC ?= gcc
CXX ?= g++
PROGRAM = forecast
CXXFILES = main.cc constants.cc mainapp.cc skeletonapplication.cc jsonparser.cc settings.cc cities.cc db.cc canvas.cc treeview_withpopup.cc forecastapi.cc
BUILT_SRC = resources.c
OBJS = $(BUILT_SRC:.c=.o) $(CXXFILES:.cc=.o)
DEPDIR = .deps
CXXFLAGS = `pkg-config --cflags gtkmm-3.0` -std=c++11 -Wall -g 
CFLAGS = `pkg-config --cflags gtk+-3.0`
LDFLAGS = -ljsoncpp -lpthread -lsqlite3 -lcurl
LIBS = `pkg-config --libs gtkmm-3.0`
GLIB_COMPILE_RESOURCES = $(shell pkg-config --variable=glib_compile_resources gio-2.0)
GLIB_COMPILE_SCHEMAS   = $(shell pkg-config --variable=glib_compile_schemas gio-2.0)
BIN_FOLDER = bin
RESOURCES_FILE_NAME = city.list.json

all: $(PROGRAM) gschemas.compiled resource.download

-include $(OBJS:%.o=$(DEPDIR)/%.Po)



gschemas.compiled: ir.youconnect.forecast.gschema.xml
	$(GLIB_COMPILE_SCHEMAS) --strict --dry-run --schema-file=$<
	$(GLIB_COMPILE_SCHEMAS) .


resources.c: forecast.gresource.xml $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=. --generate-dependencies forecast.gresource.xml)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=. --generate-source $<

%.o: %.c
	@mkdir -p $(DEPDIR)
	$(CC) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $(CFLAGS) $<
	@mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po

%.o: %.cc
	$(info $*.Tpo)
	@mkdir -p $(DEPDIR)
	$(CXX) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $(CXXFLAGS) $<
	@mv -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po


$(PROGRAM): $(OBJS)
ifneq ("$(wildcard $(BIN_FOLDER))","")
	
else
	@mkdir -p $(BIN_FOLDER)
endif
	$(CXX) -o "./bin/$(PROGRAM)" $(OBJS) $(LIBS) $(LDFLAGS)

clean:
	rm -f gschemas.compiled
	rm -f $(BUILT_SRC)
	rm -f $(OBJS)
	rm -f $(PROGRAM)
	rm -rf $(BIN_FOLDER)

install-desktop-file:
	@mkdir -p ~/.local/share/applications
	sed -e "s#@bindir@#$$PWD#" ../step1/exampleapp.desktop > ~/.local/share/applications/$(PROGRAM).desktop
	@mkdir -p ~/.local/share/icons/hicolor/48x48/apps
	cp ../step1/exampleapp.png ~/.local/share/icons/hicolor/48x48/apps

install-gschema-file:
	if test -n "$(GSETTINGS_SCHEMA_DIR)" -a "$(GSETTINGS_SCHEMA_DIR)" != .; then \
		mkdir -p "$(GSETTINGS_SCHEMA_DIR)"; \
		cp org.gtkmm.exampleapp.gschema.xml "$(GSETTINGS_SCHEMA_DIR)"; \
		$(GLIB_COMPILE_SCHEMAS) "$(GSETTINGS_SCHEMA_DIR)"; \
	fi



resource.download:

ifneq ("$(wildcard $(RESOURCES_FILE_NAME))","")
#echo "$(RESOURCES_FILE_NAME) already exists"
else
#echo "$(RESOURCES_FILE_NAME) does not exist"
ifneq ("$(wildcard $(RESOURCES_FILE_NAME).gz )","")
#echo "$(RESOURCES_FILE_NAME).gz already exists"
	gunzip -f "$(RESOURCES_FILE_NAME).gz"
else 
#echo "Downloading the file"
	wget http://bulk.openweathermap.org/sample/city.list.json.gz -O "$(RESOURCES_FILE_NAME).gz"
	gunzip -f "$(RESOURCES_FILE_NAME).gz"
endif
endif	

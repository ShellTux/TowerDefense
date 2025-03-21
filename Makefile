CC  = clang
CXX = clang++

CFLAGS += -Wall -Wextra -Werror
CFLAGS += -Wno-error=unused-variable
CFLAGS += -Wno-error=unused-but-set-variable
CFLAGS += -Wno-error=unused-result
CFLAGS += -ggdb
CFLAGS += -DNOOF
CFLAGS += -I$(shell realpath include)

CXXFLAGS += $(CFLAGS) -std=c++17

LDFLAGS = -lm -lGL -lGLU -lglfw

HEADERS = $(shell find $(INCLUDE_DIR) -name '*.h')

TARGET = td
SRC = \
      ./src/main.cpp \
      ./src/types.cpp \
      ./src/stb_image.cpp \
      ./src/Texture.cpp \
      ./src/Math.cpp \
      ./src/Color.cpp \
      ./src/Vec3.cpp \
      ./src/OBJModel.cpp \
      ./src/Primitives/3D/core.cpp \
      ./src/Primitives/2D/core.cpp \
      ./src/OpenGL/Camera.cpp \
      ./src/OpenGL/Cull.cpp \
      ./src/OpenGL/Material.cpp \
      ./src/OpenGL/PolygonMode.cpp \
      ./src/App/Base.cpp \
      ./src/App/App.cpp \
      ./src/App/Events.cpp \
      ./src/TowerDefense/Stats.cpp \
      ./src/TowerDefense/Enemy.cpp \
      ./src/TowerDefense/Cannon.cpp \
      ./src/TowerDefense/Tower.cpp \
      ./src/TowerDefense/Field.cpp
OBJ := $(SRC)
OBJ := $(OBJ:.c=.o)
OBJ := $(OBJ:.cpp=.o)

all: $(TARGET)

$(OBJ): $(HEADERS)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.pdf: %.md
	-pandoc $(PANDOC_OPTS) --from=markdown-implicit_figures --output=$@ $<

.PHONY: run
run: $(TARGET)
	./$^

.PHONY: clean
clean:
	find . -type f -iname "*.o" -exec rm "{}" \;

ARCHIVE = CG_LEI_2024_PROJETO_META_02_2018280716.zip

.PHONY: archive
archive: README.pdf
	rm --force $(ARCHIVE)
	for d in src include assets ; do (cd "$$d" && zip --recurse-paths ../$(ARCHIVE) . --exclude '**/*.o' --exclude '*.o') ; done

.PHONY: .clangd
.clangd:
	rm --force $@

	@echo Diagnostics: | tee --append $@
	@echo '  UnusedIncludes: Strict' | tee --append $@
	@echo '  MissingIncludes: Strict' | tee --append $@
	@echo CompileFlags: | tee --append $@
	@echo '  Add:' | tee --append $@

	@for flag in $(CXXFLAGS) ; do echo "    - $$flag" | tee --append $@ ; done

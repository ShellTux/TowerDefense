CXX       = clang++
CXXFLAGS += -Wall -Wextra -Werror -std=c++17
CXXFLAGS += -Wno-error=unused-variable
CXXFLAGS += -Wno-error=unused-but-set-variable
CXXFLAGS += -I$(shell realpath include)

LDFLAGS = -lm -lGL -lGLU -lglfw

all: td

TARGET = td
SRC = \
      ./src/main.cpp \
      ./src/types.cpp \
      ./src/stb_image.cpp \
      ./src/AssetManager.cpp \
      ./src/Texture.cpp \
      ./src/Math.cpp \
      ./src/Color.cpp \
      ./src/Vec3.cpp \
      ./src/Primitives/3D/core.cpp \
      ./src/Primitives/2D/core.cpp \
      ./src/OpenGL/camera.cpp \
      ./src/OpenGL/Material.cpp \
      ./src/App/Base.cpp \
      ./src/App/App.cpp \
      ./src/App/Events.cpp \
      ./src/TowerDefense/Stats.cpp \
      ./src/TowerDefense/Enemy.cpp \
      ./src/TowerDefense/Cannon.cpp \
      ./src/TowerDefense/Tower.cpp \
      ./src/TowerDefense/Field.cpp
OBJ := $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.pdf: %.md
	pandoc $(PANDOC_OPTS) --from=markdown-implicit_figures --output=$@ $<

.PHONY: run
run: $(TARGET)
	./$^

.PHONY: clean
clean:
	find . -type f -iname "*.o" -exec rm "{}" \;

.PHONY: archive
archive: README.pdf
	git archive --verbose --output=CG_LEI_2024_PROJETO_META_01_2018280716.zip $(^:%=--add-file=%) HEAD

.PHONY: .clangd
.clangd:
	rm --force $@

	@echo Diagnostics: | tee --append $@
	@echo '  UnusedIncludes: Strict' | tee --append $@
	@echo '  MissingIncludes: Strict' | tee --append $@
	@echo CompileFlags: | tee --append $@
	@echo '  Add:' | tee --append $@

	@for flag in $(CXXFLAGS) ; do echo "    - $$flag" | tee --append $@ ; done

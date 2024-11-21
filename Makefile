CXX       = clang++
CXXFLAGS += -Wall -Wextra -Werror -std=c++17
CXXFLAGS += -Wno-error=unused-parameter
CXXFLAGS += -Wno-error=unused-variable
CXXFLAGS += -Wno-error=unused-but-set-variable
CXXFLAGS += -I$(shell realpath include)

LDFLAGS = -lGL -lGLU -lglfw

all: td

TARGET = td
SRC = \
      ./src/main.cpp \
      ./src/app.cpp \
      ./src/events.cpp \
      ./src/Math.cpp \
      ./src/Primitives/3D/core.cpp \
      ./src/Primitives/2D/core.cpp \
      ./src/OpenGL/camera.cpp \
      ./src/TowerDefense/Vec3.cpp \
      ./src/TowerDefense/Stats.cpp \
      ./src/TowerDefense/Enemy/Base.cpp \
      ./src/TowerDefense/Cannon/Base.cpp \
      ./src/TowerDefense/Tower.cpp \
      ./src/TowerDefense/Field.cpp
OBJ := $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: run
run: $(TARGET)
	./$^

.PHONY: clean
clean:
	find . -type f -iname "*.o" -exec rm "{}" \;

.PHONY: .clangd
.clangd:
	rm --force $@

	@echo Diagnostics: | tee --append $@
	@echo '  UnusedIncludes: Strict' | tee --append $@
	@echo '  MissingIncludes: Strict' | tee --append $@
	@echo CompileFlags: | tee --append $@
	@echo '  Add:' | tee --append $@

	@for flag in $(CXXFLAGS) ; do echo "    - $$flag" | tee --append $@ ; done

{
	description = "Tower Defense 3D";

	inputs = {
		nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
		openggl.url = "github:ShellTux/openGGL/develop";
	};

	outputs = { nixpkgs, openggl, ... }:
	let
		system = "x86_64-linux";
		pkgs = nixpkgs.legacyPackages.${system};
		libPath = with pkgs; lib.makeLibraryPath [
			libGL
			libxkbcommon
			wayland
			xorg.libX11
			xorg.libXcursor
			xorg.libXi
			xorg.libXrandr
		];
	in
	{
		devShells.x86_64-linux.default = pkgs.mkShell {
			nativeBuildInputs = with pkgs; [
				assimp
				clang
                                entr
				freeglut
				gcc
				gcc
				gdb
				gf
				glfw
				glm
				libGLU
				openggl.packages.${system}.openGGL
				pandoc
				texlive.combined.scheme-full
				unzip
				zip
			];

			LD_LIBRARY_PATH = libPath + "/run/opengl-driver/lib:/run/opengl-driver-32/lib";
			MAKEFLAGS = "--file=Makefile";
		};
	};
}

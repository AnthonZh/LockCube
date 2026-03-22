{
  description = "FHS environment for Python CV with pip";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };

        fhs = pkgs.buildFHSEnv {
          name = "cv-fhs-env";

          targetPkgs =
            pkgs:
            (with pkgs; [
              bash
              bashInteractive
              python3
              python3Packages.pip
              python3Packages.virtualenv
              zlib
              glib
              libGL
              stdenv.cc.cc.lib
              fontconfig
              xorg.libX11
              xorg.libSM
              xorg.libXext
              xorg.libXrender
              libxcb

              xorg.xcbutilwm
              xorg.xcbutilimage
              xorg.xcbutilkeysyms
              xorg.xcbutilrenderutil
              xorg.libxcb
              xorg.libXi
              xorg.libXtst
              xorg.libXrandr
              xorg.libXfixes
              xorg.libXcursor
              xorg.libICE

              libxkbcommon
              xcb-util-cursor
              dbus
              qt5.qtbase
            ]);

          profile = ''
            export FHS=1

            if [ ! -d ".venv" ]; then
              python -m venv .venv
            fi

            source .venv/bin/activate
          '';
        };
      in
      {
        devShells.default = fhs.env;
      }
    );
}

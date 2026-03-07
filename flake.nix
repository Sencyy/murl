{
  description = "A tiny url shortener";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: 
		let
			system = "x86_64-linux";
			pkgs = nixpkgs.legacyPackages.${system};
		in {
    packages.${system} = rec {
			default = murl;

			murl = pkgs.stdenv.mkDerivation {
				pname = "murl";
				version = "1.0";

				src = ./.;

				nativeBuildInputs = with pkgs; [ # Build dependencies
					gcc
					make
				];
				buildInputs = with pkgs; [ # Runtime dependencies
					stb
				];

				buildPhase = "make";
			} 
    packages.x86_64-linux.default = self.packages.x86_64-linux.murl;

    };
  };
}

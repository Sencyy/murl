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

			murl = pkgs.buildGoModule {
				name = "murl";
				src = ./src/backend;
				vendorHash = null;
				buildInputs = with pkgs; [];
			};

    packages.x86_64-linux.default = self.packages.x86_64-linux.murl;

    };
  };
}

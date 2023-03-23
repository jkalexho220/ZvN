void modifyPlatform(int x = 0, int z = 0, float height = 0) {
	//trPaintTerrain(x - 1, z - 1, x + 1, z + 1, 0, secondary, false);
	trChangeTerrainHeight(x - 1, z - 1, x + 1, z + 1, height, false);
}

rule terraform
active
highFrequency
{
	xsDisableSelf();
	int center = 16;
	int dist = 0;
	float height = 0;

	for(x=0; <= 5) {
		for(z=0; <= 5) {
			dist = x + z - 5;
			if (dist < 0) {
				height = 9;
			} else {
				height = xsMax(-3.0, 6.0 - 3.0 * dist);
			}
			modifyPlatform(center - 3 * x, center - 3 * z, height);
			modifyPlatform(center - 3 * x, center + 3 * z, height);
			modifyPlatform(center + 3 * x, center - 3 * z, height);
			modifyPlatform(center + 3 * x, center + 3 * z, height);
		}
	}
	for(x=0; <= 32) {
		for(z=0; <= 32) {
			height = trGetTerrainHeight(x, z) + trGetTerrainHeight(x + 1, z + 1);
			if (height < 18.0) {
				trPaintTerrain(x, z, x, z, 2, 1, false);
			} else {
				trPaintTerrain(x, z, x, z, 0, 53, false);
			}
		}
	}
}
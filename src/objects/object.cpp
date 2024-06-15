#include "../Headers/objects.h"
#include <gl\GL.h>


void Terrain::Draw() {
        glTranslatef(pos.x, pos.y, pos.z);
        float stepx = (float)size.x / (float)res.x;
        float stepy = (float)size.y / (float)res.y;

        float pix_x = (float)heightmap->getSize().x / (float)res.x;
        float pix_y = (float)heightmap->getSize().y / (float)res.y;
        for (int i = 0; i < res.x - 1; i++) {
            for (int j = 0; j < res.y - 2; j++) {
                
                Vector3f points[4]; {
                    points[0] = Vector3f(i * stepx, (float)(heightmap->getPixel(pix_x * i, pix_y * j).r) * scale, stepy * j);
                    points[1] = Vector3f((i + 1) * stepx, (float)(heightmap->getPixel(pix_x * (i + 1), pix_y * j).r) * scale, stepy * j);
                    points[2] = Vector3f((i + 1) * stepx, (float)(heightmap->getPixel(pix_x * (i + 1), pix_y * (j + 1)).r) * scale, stepy * (j + 1));
                    points[3] = Vector3f(i * stepx, (float)(heightmap->getPixel(pix_x * i, pix_y * (j + 1)).r) * scale, stepy * (j + 1));
                }
                bool draw = true;
                
                for (int k = 0; k < 4; k++) {
                    draw = draw && points[k].z >= 0;
                }
                if (draw) {
                    
                    DrawQuad(Vector3f(0, 0, 0), Vector3f(0, 0, 0), points, texture);
                    
                }
            }
        }
        glTranslatef(-pos.x, -pos.y, -pos.z);
}


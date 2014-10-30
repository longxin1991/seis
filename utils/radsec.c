#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265358979323846
#define DEG2RAD PI/180.0

int main (int argc, char *argv[]) {
    int i, j;
    float m[3][3];
	float az,theta;

    if (argc != 8 && argc != 5) {
        fprintf(stderr, "Usage: %s mrr mtt mff mrt mrf mtf\n", argv[0]);
        fprintf(stderr, "   Or: %s strike dip rake\n", argv[0]);
        exit(1);
    }

    if (argc == 8) {    // moment tensor
        sscanf(argv[1], "%f", &m[2][2]);    // mrr -> mzz
        sscanf(argv[2], "%f", &m[0][0]);    // mtt -> mxx
        sscanf(argv[3], "%f", &m[1][1]);    // mff -> myy

        sscanf(argv[4], "%f", &m[2][0]);    // mrt -> mzx
        m[0][2] = m[2][0];

        sscanf(argv[5], "%f", &m[2][1]);    // mrf -> -Mzy
        m[2][1] = -m[2][1];
        m[1][2] = m[2][1];

        sscanf(argv[6], "%f", &m[0][1]);    // mtf -> -Mxy
        m[0][1] = -m[0][1];
        m[1][0] = m[0][1];
		az=atof(argv[7]);
    } else if (argc == 5) { // strike, dip, rake
        float strike, dip, rake;
        sscanf(argv[1], "%f", &strike);
        sscanf(argv[2], "%f", &dip   );
        sscanf(argv[3], "%f", &rake  );
        strike *= DEG2RAD;
        rake   *= DEG2RAD;
        dip    *= DEG2RAD;
        m[0][0] = - sin(dip)*cos(rake)*sin(2*strike)
                - sin(2*dip)*sin(rake)*sin(strike)*sin(strike);
        m[0][1] = sin(dip)*cos(rake)*cos(2*strike)
                + 0.5*sin(2*dip)*sin(rake)*sin(2*strike);
        m[0][2] = -cos(dip)*cos(rake)*cos(strike)
                - cos(2*dip)*sin(rake)*sin(strike);
        m[1][1] = sin(dip)*cos(rake)*sin(2*strike)
                - sin(2*dip)*sin(rake)*cos(strike)*cos(strike);
        m[1][2] = -cos(dip)*cos(rake)*sin(strike)
                + cos(2*dip)*sin(rake)*cos(strike);
        m[2][2] = sin(2*dip)*sin(rake);
        m[1][0] = m[0][1];
        m[2][0] = m[0][2];
        m[2][1] = m[1][2];
		az=atof(argv[4]);
    }

    fprintf(stdout, "    / %6.3f %6.3f %6.3f \\ \n", m[0][0], m[0][1], m[0][2]);
    fprintf(stdout, "M = | %6.3f %6.3f %6.3f | \n",  m[1][0], m[1][1], m[1][2]);
    fprintf(stdout, "    \\ %6.3f %6.3f %6.3f / \n", m[2][0], m[2][1], m[2][2]);
	
	az *= DEG2RAD;     // 方位角
    FILE *fop;
    fop = fopen("pattern.dat", "w");
    float p[3];         // 离源矢量
      for (j=0; j<=3600; j++) {
        
        theta = j/10.0 * DEG2RAD;    // 离源角，仅计算下半球
        p[0] = (float)(sin(theta)*cos(az));
        p[1] = (float)(sin(theta)*sin(az));
        p[2] = (float)(cos(theta));

        int k, l;
        float amp = 0.0;
        for (k=0; k<=2; k++)
          for (l=0; l<=2; l++){
            amp += p[k]*m[k][l]*p[l];
        }
        //fwrite(&amp, sizeof(float), 1, fop);
        fprintf(fop,"%f %f\n",theta*180/PI,fabs(amp));
    }
    fclose(fop);

    return 0;
}

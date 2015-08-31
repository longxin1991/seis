
/*linear velocity function for each layer*/

float vl(float v1,float v2,float z1,float z2,float z)
{
	float k,v;
	k = (v2-v1)/(z2-z1);

	v = v2 + k*(z - z2);

	return v;
}

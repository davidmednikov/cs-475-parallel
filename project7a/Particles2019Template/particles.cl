typedef float4 point;
typedef float4 vector;
typedef float4 color;
typedef float4 sphere;


vector
Bounce( vector in, vector n )
{
	vector out = in - n*(vector)( 2.*dot(in.xyz, n.xyz) );
	out.w = 0.;
	return out;
}

vector
BounceSphere( point p, vector v, sphere s )
{
	vector n;
	n.xyz = fast_normalize( p.xyz - s.xyz );
	n.w = 0.;
	return Bounce( v, n );
}

bool
IsInsideSphere( point p, sphere s )
{
	float r = fast_length( p.xyz - s.xyz );
	return  ( r < s.w );
}

kernel
void
Particle( global point *dPobj, global vector *dVel, global color *dCobj )
{
	const float4 G       = (float4) ( 0., -9.8, 0., 0. );
	const float  DT      = 0.1;
	const sphere Sphere1 = (sphere)( -100., -800., 0.,  600. );
	const sphere Sphere2 = (sphere)( 600., 100., 200.,  450. );
	int gid = get_global_id( 0 );

	// extract the position and velocity for this particle:
	point  p = dPobj[gid];
	vector v = dVel[gid];
	color c = dCobj[gid];

	// remember that you also need to extract this particle's color
	// and change it in some way that is obviously correct


	// advance the particle:

	point  pp = p + v*DT + G*(point)( .5*DT*DT );
	vector vp = v + G*DT;
	color cp = c;
	bool bounced = false;
	pp.w = 1.;
	vp.w = 0.;

	// test against the first sphere here:

	if( IsInsideSphere( pp, Sphere1 ) )
	{
		vp = BounceSphere( p, v, Sphere1 );
		pp = p + vp*DT + G*(point)( .5*DT*DT );
		cp.x = 1.;
		cp.y = .341;
		cp.z = .133;
		bounced = true;
	}

	// test against the second sphere here:

	if( IsInsideSphere( pp, Sphere2 ) )
	{
	        vp = BounceSphere( p, v, Sphere2 );
	        pp = p + vp*DT + G*(point)( .5*DT*DT );
	        cp.x = .553;
	        cp.y = .133;
	        cp.z = 1.;
	        bounced = true;
	}

	dPobj[gid] = pp;
	dVel[gid]  = vp;
	if (bounced) {
	    dCobj[gid] = cp;
	}
	if (pp.y < -4000) {
		cp.x = .075;
	    cp.y = .89;
	    cp.z = .067;
		dCobj[gid] = cp;
	}
}

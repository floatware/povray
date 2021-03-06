/*******************************************************************************
 * sphsweep.h
 *
 * Contributed by Jochen Lippert
 *
 * This module contains the global defines, typedefs, and prototypes
 * for sphsweep.cpp.
 *
 * ---------------------------------------------------------------------------
 * Persistence of Vision Ray Tracer ('POV-Ray') version 3.7.
 * Copyright 1991-2013 Persistence of Vision Raytracer Pty. Ltd.
 *
 * POV-Ray is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * POV-Ray is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ---------------------------------------------------------------------------
 * POV-Ray is based on the popular DKB raytracer version 2.12.
 * DKBTrace was originally written by David K. Buck.
 * DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
 * ---------------------------------------------------------------------------
 * $File: //depot/public/povray/3.x/source/backend/shape/sphsweep.h $
 * $Revision: #1 $
 * $Change: 6069 $
 * $DateTime: 2013/11/06 11:59:40 $
 * $Author: chrisc $
 *******************************************************************************/

#ifndef SPHERE_SWEEP_H
#define SPHERE_SWEEP_H

namespace pov
{

/*****************************************************************************
* Global preprocessor defines
******************************************************************************/

#define SPHERE_SWEEP_OBJECT     (BASIC_OBJECT)

/* Sphere sweep interpolated by a piecewise linear function */
#define LINEAR_SPHERE_SWEEP             0

/* Sphere sweep interpolated by a cubic Catmull-Rom-Spline function */
#define CATMULL_ROM_SPLINE_SPHERE_SWEEP 1

/* Sphere sweep approximated by a cubic B-Spline function */
#define B_SPLINE_SPHERE_SWEEP           2

/* Maximum number of coefficients of the polynomials describing one segment */
#define SPH_SWP_MAX_COEFS               4



/*****************************************************************************
* Global typedefs
******************************************************************************/

typedef struct Sphere_Sweep_Sphere_Struct SPHSWEEP_SPH;
typedef struct Sphere_Sweep_Segment_Struct SPHSWEEP_SEG;
typedef struct Sphere_Sweep_Intersection_Structure SPHSWEEP_INT;

/* Single sphere, used to connect two adjacent segments */
struct Sphere_Sweep_Sphere_Struct
{
	VECTOR      Center;
	DBL         Radius;
};

/* One segment of the sphere sweep */
struct Sphere_Sweep_Segment_Struct
{
	SPHSWEEP_SPH  Closing_Sphere[2];        /* Spheres closing the segment   */
	VECTOR  Center_Deriv[2];    /* Derivatives of center funcs for 0 and 1   */
	DBL     Radius_Deriv[2];    /* Derivatives of radius funcs for 0 and 1   */
	int     Num_Coefs;                      /* Number of coefficients        */
	VECTOR  Center_Coef[SPH_SWP_MAX_COEFS]; /* Coefs of center polynomial    */
	DBL     Radius_Coef[SPH_SWP_MAX_COEFS]; /* Coefs of radius polynomial    */
};

// Temporary storage for intersection values
struct Sphere_Sweep_Intersection_Structure
{
	DBL     t;          // Distance along ray
	VECTOR  Point;      // Intersection point
	VECTOR  Normal;     // Normal at intersection point
};

/* The complete object */
class SphereSweep : public ObjectBase
{
	public:
		int             Interpolation;
		int             Num_Modeling_Spheres;   /* Number of modeling spheres    */
		SPHSWEEP_SPH    *Modeling_Sphere;       /* Spheres describing the shape  */
		int             Num_Spheres;            /* Number of single spheres      */
		SPHSWEEP_SPH    *Sphere;                /* Spheres that close segments   */
		int             Num_Segments;           /* Number of tubular segments    */
		SPHSWEEP_SEG    *Segment;               /* Tubular segments              */
		DBL             Depth_Tolerance;        /* Preferred depth tolerance     */

		SphereSweep();
		virtual ~SphereSweep();

		virtual ObjectPtr Copy();

		virtual bool All_Intersections(const Ray&, IStack&, TraceThreadData *);
		virtual bool Inside(const VECTOR, TraceThreadData *) const;
		virtual void Normal(VECTOR, Intersection *, TraceThreadData *) const;
		virtual void Translate(const VECTOR, const TRANSFORM *);
		virtual void Rotate(const VECTOR, const TRANSFORM *);
		virtual void Scale(const VECTOR, const TRANSFORM *);
		virtual void Transform(const TRANSFORM *);
		virtual void Invert();
		virtual void Compute_BBox();

		void Compute();
	protected:
		bool Intersect(Ray& ray, VECTOR Center, DBL Radius2, DBL *Depth1, DBL *Depth2);
		static bool Intersect_Sphere(const Ray &ray, const SPHSWEEP_SPH *Sphere, SPHSWEEP_INT *Isect);
		static int Intersect_Segment(const Ray &ray, const SPHSWEEP_SEG *Segment, SPHSWEEP_INT *Isect, TraceThreadData *Thread);
		static int Find_Valid_Points(SPHSWEEP_INT *Inter, int Num_Inter, const Ray &ray);
		static int Comp_Isects(const void *Intersection_1, const void *Intersection_2);
		static int bezier_01(int degree, const DBL* Coef, DBL* Roots, bool sturm, DBL tolerance, TraceThreadData *Thread);
};

}

#endif

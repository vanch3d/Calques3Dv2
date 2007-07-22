//////////////////////////////////////////////////////////////////////
// Calques 3D - a 3D Dynamic Geometry Learning Environment
// Copyright (c) 1997-2007 Nicolas Van Labeke
//////////////////////////////////////////////////////////////////////
// This file is part of Calques 3D.
// 
// Calques 3D is free software; you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by 
// the Free Software Foundation; either version 2 of the License, or 
// (at your option) any later version.
// 
// Calques 3D is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License 
// along with Calques 3D; if not, write to The Free Software Foundation, Inc., 
// 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA 
//////////////////////////////////////////////////////////////////////
// glut.cpp: implementation of the GLUT functions.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OGLT.h"
#include "glut.h"
#include <math.h>

#define M_PI	3.1415926535	///< Internal encoding for PI

/*
 * Draws a wire torus
 */
void glutWireTorus( GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings )
{
	  double  iradius = dInnerRadius, oradius = dOuterRadius, phi, psi, dpsi, dphi;
	  double *vertex, *normal;
	  int    i, j;
	  double spsi, cpsi, sphi, cphi ;

	  if ( nSides < 1 ) nSides = 1;
	  if ( nRings < 1 ) nRings = 1;

	  /* Allocate the vertices array */
	  vertex = (double *)calloc( sizeof(double), 3 * nSides * nRings );
	  normal = (double *)calloc( sizeof(double), 3 * nSides * nRings );

	  glPushMatrix();

	  dpsi =  2.0 * M_PI / (double)nRings ;
	  dphi = -2.0 * M_PI / (double)nSides ;
	  psi  = 0.0;

	  for( j=0; j<nRings; j++ )
	  {
		cpsi = cos ( psi ) ;
		spsi = sin ( psi ) ;
		phi = 0.0;

		for( i=0; i<nSides; i++ )
		{
		  int offset = 3 * ( j * nSides + i ) ;
		  cphi = cos ( phi ) ;
		  sphi = sin ( phi ) ;
		  *(vertex + offset + 0) = cpsi * ( oradius + cphi * iradius ) ;
		  *(vertex + offset + 1) = spsi * ( oradius + cphi * iradius ) ;
		  *(vertex + offset + 2) =                    sphi * iradius  ;
		  *(normal + offset + 0) = cpsi * cphi ;
		  *(normal + offset + 1) = spsi * cphi ;
		  *(normal + offset + 2) =        sphi ;
		  phi += dphi;
		}

		psi += dpsi;
	  }

	  for( i=0; i<nSides; i++ )
	  {
		glBegin( GL_LINE_LOOP );

		for( j=0; j<nRings; j++ )
		{
		  int offset = 3 * ( j * nSides + i ) ;
		  glNormal3dv( normal + offset );
		  glVertex3dv( vertex + offset );
		}

		glEnd();
	  }

	  for( j=0; j<nRings; j++ )
	  {
		glBegin(GL_LINE_LOOP);

		for( i=0; i<nSides; i++ )
		{
		  int offset = 3 * ( j * nSides + i ) ;
		  glNormal3dv( normal + offset );
		  glVertex3dv( vertex + offset );
		}

		glEnd();
	  }

	  free ( vertex ) ;
	  free ( normal ) ;
	  glPopMatrix();
}

void glutSolidTorus( GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings )
{
  double  iradius = dInnerRadius, oradius = dOuterRadius, phi, psi, dpsi, dphi;
  double *vertex, *normal;
  int    i, j;
  double spsi, cpsi, sphi, cphi ;

  if ( nSides < 1 ) nSides = 1;
  if ( nRings < 1 ) nRings = 1;

  /* Increment the number of sides and rings to allow for one more point than surface */
  nSides ++ ;
  nRings ++ ;

  /* Allocate the vertices array */
  vertex = (double *)calloc( sizeof(double), 3 * nSides * nRings );
  normal = (double *)calloc( sizeof(double), 3 * nSides * nRings );

  glPushMatrix();

  dpsi =  2.0 * M_PI / (double)(nRings - 1) ;
  dphi = -2.0 * M_PI / (double)(nSides - 1) ;
  psi  = 0.0;

  for( j=0; j<nRings; j++ )
  {
    cpsi = cos ( psi ) ;
    spsi = sin ( psi ) ;
    phi = 0.0;

    for( i=0; i<nSides; i++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      cphi = cos ( phi ) ;
      sphi = sin ( phi ) ;
      *(vertex + offset + 0) = cpsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 1) = spsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 2) =                    sphi * iradius  ;
      *(normal + offset + 0) = cpsi * cphi ;
      *(normal + offset + 1) = spsi * cphi ;
      *(normal + offset + 2) =        sphi ;
      phi += dphi;
    }

    psi += dpsi;
  }

    glBegin( GL_QUADS );
  for( i=0; i<nSides-1; i++ )
  {
    for( j=0; j<nRings-1; j++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      glNormal3dv( normal + offset );
      glVertex3dv( vertex + offset );
      glNormal3dv( normal + offset + 3 );
      glVertex3dv( vertex + offset + 3 );
      glNormal3dv( normal + offset + 3 * nSides + 3 );
      glVertex3dv( vertex + offset + 3 * nSides + 3 );
      glNormal3dv( normal + offset + 3 * nSides );
      glVertex3dv( vertex + offset + 3 * nSides );
    }
  }

  glEnd();

  free ( vertex ) ;
  free ( normal ) ;
  glPopMatrix();
}

/* Magic Numbers:  r0 = ( 1, 0, 0 )
 *                 r1 = ( -1/3, 2 sqrt(2) / 3, 0 )
 *                 r2 = ( -1/3, -sqrt(2) / 3, sqrt(6) / 3 )
 *                 r3 = ( -1/3, -sqrt(2) / 3, -sqrt(6) / 3 )
 * |r0| = |r1| = |r2| = |r3| = 1
 * Distance between any two points is 2 sqrt(6) / 3
 *
 * Normals:  The unit normals are simply the negative of the coordinates of the point not on the surface.
 */


#define NUM_TETR_FACES     4

static GLdouble tet_r[4][3] = { {             1.0,             0.0,             0.0 },
                                { -0.333333333333,  0.942809041582,             0.0 },
                                { -0.333333333333, -0.471404520791,  0.816496580928 },
                                { -0.333333333333, -0.471404520791, -0.816496580928 } } ;

static GLint tet_i[4][3] =  /* Vertex indices */
{
  { 1, 3, 2 }, { 0, 2, 3 }, { 0, 3, 1 }, { 0, 1, 2 }
} ;

/*
 *
 */
void glutWireTetrahedron( void )
{
  glBegin( GL_LINE_LOOP ) ;
    glNormal3d ( -tet_r[0][0], -tet_r[0][1], -tet_r[0][2] ) ; glVertex3dv ( tet_r[1] ) ; glVertex3dv ( tet_r[3] ) ; glVertex3dv ( tet_r[2] ) ;
    glNormal3d ( -tet_r[1][0], -tet_r[1][1], -tet_r[1][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[2] ) ; glVertex3dv ( tet_r[3] ) ;
    glNormal3d ( -tet_r[2][0], -tet_r[2][1], -tet_r[2][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[3] ) ; glVertex3dv ( tet_r[1] ) ;
    glNormal3d ( -tet_r[3][0], -tet_r[3][1], -tet_r[3][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[1] ) ; glVertex3dv ( tet_r[2] ) ;
  glEnd() ;
}

/*
 *
 */
void glutSolidTetrahedron( void )
{
  glBegin( GL_TRIANGLES ) ;
    glNormal3d ( -tet_r[0][0], -tet_r[0][1], -tet_r[0][2] ) ; glVertex3dv ( tet_r[1] ) ; glVertex3dv ( tet_r[3] ) ; glVertex3dv ( tet_r[2] ) ;
    glNormal3d ( -tet_r[1][0], -tet_r[1][1], -tet_r[1][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[2] ) ; glVertex3dv ( tet_r[3] ) ;
    glNormal3d ( -tet_r[2][0], -tet_r[2][1], -tet_r[2][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[3] ) ; glVertex3dv ( tet_r[1] ) ;
    glNormal3d ( -tet_r[3][0], -tet_r[3][1], -tet_r[3][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[1] ) ; glVertex3dv ( tet_r[2] ) ;
  glEnd() ;
}


/*
 * Compute lookup table of cos and sin values forming a cirle
 *
 * Notes:
 *    It is the responsibility of the caller to free these tables
 *    The size of the table is (n+1) to form a connected loop
 *    The last entry is exactly the same as the first
 *    The sign of n can be flipped to get the reverse loop
 */

static void fghCircleTable(double **sint,double **cost,const int n)
{
    int i;

    /* Table size, the sign of n flips the circle direction */

    const int size = abs(n);

    /* Determine the angle between samples */

    const double angle = 2*M_PI/(double)( ( n == 0 ) ? 1 : n );

    /* Allocate memory for n samples, plus duplicate of first entry at the end */

    *sint = (double *) calloc(sizeof(double), size+1);
    *cost = (double *) calloc(sizeof(double), size+1);

    /* Bail out if memory allocation fails, fgError never returns */

    if (!(*sint) || !(*cost))
    {
        free(*sint);
        free(*cost);
        //fgError("Failed to allocate memory in fghCircleTable");
    }

    /* Compute cos and sin around the circle */

    (*sint)[0] = 0.0;
    (*cost)[0] = 1.0;

    for (i=1; i<size; i++)
    {
        (*sint)[i] = sin(angle*i);
        (*cost)[i] = cos(angle*i);
    }

    /* Last sample is duplicate of the first */

    (*sint)[size] = (*sint)[0];
    (*cost)[size] = (*cost)[0];
}

/*
 * Draws a solid cone
 */
void glutSolidCone( GLdouble base, GLdouble height, GLint slices, GLint stacks )
{
    int i,j;

    /* Step in z and radius as stacks are drawn. */

    double z0,z1;
    double r0,r1;

    const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );
    const double rStep = base / ( ( stacks > 0 ) ? stacks : 1 );

    /* Scaling factors for vertex normals */

    const double cosn = ( height / sqrt ( height * height + base * base ));
    const double sinn = ( base   / sqrt ( height * height + base * base ));

    /* Pre-computed circle */

    double *sint,*cost;

    fghCircleTable(&sint,&cost,-slices);

    /* Cover the circular base with a triangle fan... */

    z0 = 0.0;
    z1 = zStep;

    r0 = base;
    r1 = r0 - rStep;

    glBegin(GL_TRIANGLE_FAN);

        glNormal3d(0.0,0.0,-1.0);
        glVertex3d(0.0,0.0, z0 );

        for (j=0; j<=slices; j++)
            glVertex3d(cost[j]*r0, sint[j]*r0, z0);

    glEnd();

    /* Cover each stack with a quad strip, except the top stack */

    for( i=0; i<stacks-1; i++ )
    {
        glBegin(GL_QUAD_STRIP);

            for(j=0; j<=slices; j++)
            {
                glNormal3d(cost[j]*sinn, sint[j]*sinn, cosn);
                glVertex3d(cost[j]*r0,   sint[j]*r0,   z0  );
                glVertex3d(cost[j]*r1,   sint[j]*r1,   z1  );
            }

            z0 = z1; z1 += zStep;
            r0 = r1; r1 -= rStep;

        glEnd();
    }

    /* The top stack is covered with individual triangles */

    glBegin(GL_TRIANGLES);

        glNormal3d(cost[0]*sinn, sint[0]*sinn, cosn);

        for (j=0; j<slices; j++)
        {
            glVertex3d(cost[j+0]*r0,   sint[j+0]*r0,   z0    );
            glVertex3d(0,              0,              height);
            glNormal3d(cost[j+1]*sinn, sint[j+1]*sinn, cosn  );
            glVertex3d(cost[j+1]*r0,   sint[j+1]*r0,   z0    );
        }

    glEnd();

    /* Release sin and cos tables */

    free(sint);
    free(cost);
}

/*
 * Draws a wire cone
 */
void glutWireCone( GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
    int i,j;

    /* Step in z and radius as stacks are drawn. */

    double z = 0.0;
    double r = base;

    const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );
    const double rStep = base / ( ( stacks > 0 ) ? stacks : 1 );

    /* Scaling factors for vertex normals */

    const double cosn = ( height / sqrt ( height * height + base * base ));
    const double sinn = ( base   / sqrt ( height * height + base * base ));

    /* Pre-computed circle */

    double *sint,*cost;

    fghCircleTable(&sint,&cost,-slices);

    /* Draw the stacks... */

    for (i=0; i<stacks; i++)
    {
        glBegin(GL_LINE_LOOP);

            for( j=0; j<slices; j++ )
            {
                glNormal3d(cost[j]*sinn, sint[j]*sinn, cosn);
                glVertex3d(cost[j]*r,    sint[j]*r,    z   );
            }

        glEnd();

        z += zStep;
        r -= rStep;
    }

    /* Draw the slices */

    r = base;

    glBegin(GL_LINES);

        for (j=0; j<slices; j++)
        {
            glNormal3d(cost[j]*sinn, sint[j]*sinn, cosn  );
            glVertex3d(cost[j]*r,    sint[j]*r,    0.0   );
            glVertex3d(0.0,          0.0,          height);
        }

    glEnd();

    /* Release sin and cos tables */

    free(sint);
    free(cost);
}

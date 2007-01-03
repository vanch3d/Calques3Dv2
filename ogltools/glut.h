
#define RTD(x)	((x*180)/M_PI)

void glutWireTetrahedron( void );
void glutSolidTetrahedron( void );
void glutWireTorus( GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings );
void glutSolidTorus( GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings );

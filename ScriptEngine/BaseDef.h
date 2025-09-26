
#ifndef _BaseDef_H_
#define _BaseDef_H_


#define MACHINECODEFILEEXT	".mcf"						//Machine Code File
#define INTCODEFILEEXT		".imc"						//InterMediate Code
#define SCRIPTFILEEXT		".gsf"						//Gama Script File

///////////////////////////////////////////////////////////////////////////////////
//

typedef void *	ANY_FUNCTION;

enum eDataType
{
	T_VOID = 0,
	T_BOOL,
	T_INT,
	T_FLOAT,
	T_STRING = 4
};

typedef int	BOOL;

#define	TRUE	1
#define FALSE	0

///////////////////////////////////////////////////////////////////////////////////
//

struct ScriptFunc
{
	void *	pFunc;
	long	Type;

	ScriptFunc( void *, long );
};

typedef ScriptFunc	SE_FUNC;

///////////////////////////////////////////////////////////////////////////////////
//

union AnyData
{
	int			intValue;
	float		floatValue;
	const char*	stringValue;

	AnyData( int n )
		: intValue( n )
	{}

	AnyData( float f )
		: floatValue( f )
	{}

	AnyData( const char * pszStr )
		: stringValue( pszStr )
	{}

	AnyData( void * p )
		: stringValue( (const char *)p )
	{
	}

	AnyData &	operator=( int n )
	{
		intValue = n;
		return *this;
	}
};


///////////////////////////////////////////////////////////////////////////////////

#endif
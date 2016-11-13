int addition(int nOne, int nTwo)
{
	int carryover;
	// while there is in the operands
	// exclusive or operation
	// and operation
	// Bitshift and start the process all over again
	// cout<< The result of the operations (ADDITION)
	while (nTwo != 0)
	{
		carryover = nOne&nTwo;
		nOne = nOne^nTwo;
		nTwo = carryover << 1;
	}
	return nOne;
}
int multiplication(int nOne, int nTwo)
{


	int result = 0;
	while (nTwo)
	{
		if (nTwo & 1)
			result = addition(result, nOne);

		nOne = nOne << 1;
		nTwo = nTwo >> 1;

	}




	return result;
}

int subtraction(int nOne,int nTwo)
{

	//cin >> nOne >> nTwo;
	int borrow;
	// while there is in the operands
	// exclusive or operation
	// and operation
	// Bitshift and start the process all over again
	// cout<< The result of the operations (ADDITION)
	while (nTwo != 0)
	{
		borrow = (~nOne)&nTwo;
		nOne = nOne^nTwo;
		nTwo = borrow << 1;
	}




	return nOne;
}

void division(int nOne, int nTwo,int *quotient, int *remainder)
{
	int dividend = nOne, divisor = nTwo;
	int q;

	*quotient = 0;
	*remainder = 0;

	while (dividend >= divisor)
	{
		q = 1;

		while ( (divisor<<1) <= dividend)
		{
			divisor=divisor << 1;
			q=q << 1;
		}

		dividend = subtraction(dividend, divisor);
		*quotient = addition(*quotient, q);

		divisor = nTwo;
	}

	*remainder = dividend;
}

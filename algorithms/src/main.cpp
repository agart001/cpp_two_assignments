#include <iostream>
#include "../include/types.hpp"
#include "../include/algo.hpp"

/*
* Exploration and Data testing 
*/
/*long long int run_product = 1;
long long int prev_product = 0;
long long int high_product = 0;
int sec_index = 0;
int sec_val_index = 0;
for (size_t i = 0; i < nums->len; i++)
{

	if (i % 13 == 0 && i != 0)
	{

		prev_product = run_product;
		run_product = 1;

		std::cout << " | Section #" << sec_index << ": " << prev_product << " | " << std::endl;
		sec_index++;
		sec_val_index = 0;
	}

	if (prev_product > high_product)
	{
		high_product = prev_product;
		std::cout << " | New High! | Section #" << sec_index << ": " << high_product << " | " << std::endl;
	}

	int _data = nums->data[i];
	run_product = run_product * _data;

	std::cout
		<< " | Section #" << sec_index
		<< " | Sec Index #" << sec_val_index
		<< " | Act Index #" << i
		<< " | Data Value: " << _data
		<< " | Running Prod: " << run_product
		<< std::endl;

	sec_val_index++;

	if (i % 13 == 0 && i != 0)
	{

		prev_product = run_product;
		run_product = 1;

		std::cout << "Section #" << sec_index << ": " << prev_product << std::endl;
		sec_index++;
		sec_val_index = 0;
	}

	if (prev_product > high_product)
	{
		high_product = prev_product;
	}
}*/




/*for (int i = 0; i < _input.length(); i += 14)
{

	int e = i + 14;
	std::cout << "Start Index: " << i << std::endl;
	std::cout << "End Index: " << e << std::endl;
	std::string sec = "";

	sec = _input.substr(i, 14);
	if (i <= 0)
	{
		sec = _input.substr(i, 14);
	}
	else
	{
		sec = _input.substr(i-1, 14);
	}
	//std::cout << sec << std::endl;

	char secchars[14];
	std::copy(sec.begin(), sec.end(), secchars);

	for (int j = 0; j < 14; j++)
	{
		int asInt = secchars[j] - '0';
		std::cout << "Sec Char #" << j << "as Int: " << asInt << std::endl;
	}
}*/

//First Implementation - Wrong
// Only checks a static window which is dependant upon
// the index being a factor of the group size
/*long long int GetHighestProduct(Array* nums, const int& count)
{
	long long int run_product = 1;
	long long int prev_product = 0;
	long long int high_product = 0;
	for (size_t i = 0; i < nums->len; i++)
	{

		if (i % count == 0 && i != 0)
		{

			prev_product = run_product;
			run_product = 1;
		}

		if (prev_product > high_product)
		{
			high_product = prev_product;
		}

		int _data = nums->data[i];
		run_product = run_product * _data;
	}

	return high_product;
}*/

/*
* Second Implementation 
* incorrectly gets the starts window, does not properly handle zeros in the window
* 
*/
/*int GetHighestProduct(Array* nums, int count)
{
	int product = 1;
	int hiproduct = 0;
	int old_digit = 0;
	int new_digit = 0;

	for (size_t i = 0; i < nums->len; i++)
	{
		new_digit = nums->data[i];

		if (i < count + 1)
		{
			product = product * new_digit;
		}
		else
		{
			old_digit = nums->data[i - count];

			if (old_digit != 0)
			{
				product = product / old_digit;
			}

			product = product * new_digit;
		}

		if (product > hiproduct)
		{
			hiproduct = product;
		}

	}

	return hiproduct;
}*/


/*
* Final Implementation
* Gets correct starting window
* Properly handles zeros, only updates product without zero in window
*/
long long int GetHighestProduct_Main(Array* nums, int count)
{
	long long int product = 1;
	long long int hiproduct = 0;
	int z_cnt = 0;

	for (int i = 0; i < count; i++)
	{
		int digit = nums->data[i];
		if (digit == 0)
		{
			z_cnt++;
		}
		else
		{
			product = product * digit;
		}
	}
	
	if (z_cnt == 0)
	{
		hiproduct = product;
	}


	for (size_t i = 0; i < nums->len; i++)
	{
		int digit_to_add = nums->data[i];
		int digit_to_remove= nums->data[i - count];

		if (digit_to_remove == 0)
		{
			z_cnt--;
		}
		else
		{
			product = product / digit_to_remove;
		}

		if (digit_to_add == 0)
		{
			z_cnt++;
		}
		else
		{
			product = product * digit_to_add;
		}

		if (z_cnt == 0 && product > hiproduct)
		{
			hiproduct = product;
		}

	}

	return hiproduct;
}

Array* DigitStringToIntArray_Main(const std::string& digits)
{
	size_t _len = digits.length();
	int* _data = (int*)safe_alloc(_len, sizeof(int));
	for (size_t i = 0; i < _len; i++)
	{
		_data[i] = digits[i] - '0';
	}

	return init(_len, _data);
}

auto main([[maybe_unused]] int argc, [[maybe_unused]]  char* argv[]) -> int {

	/*std::string _input = input;
	_input.erase(std::remove(_input.begin(), _input.end(), '\n'), _input.end());
	std::cout << _input.length() << std::endl;

	Array* nums = DigitStringToIntArray(_input);

	int hp = GetHighestProduct(nums, 13);
	std::cout << hp << std::endl;*/


    return 0;
}


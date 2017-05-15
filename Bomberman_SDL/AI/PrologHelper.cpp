#include "PrologHelper.h"

#include <iostream>
#include <fstream>

using namespace std;

bool g_initAi = false;

void PrologHelper::Init(int w, int h)
{
	if (g_initAi)
	{
		predicate_t pr;

		term_t t0 = PL_new_term_refs(2);

		int rval;

		PL_put_integer(t0, w);
		PL_put_integer(t0 + 1, h);

		pr = PL_predicate("init", 2, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);

		cout << "Init Result -> " << rval << ";" << endl;
	}
}

void PrologHelper::AgentLocation(int x, int y)
{
	if (g_initAi)
	{
		predicate_t pr;

		term_t t0 = PL_new_term_refs(2);

		int rval;

		PL_put_integer(t0, x);
		PL_put_integer(t0 + 1, y);

		pr = PL_predicate("set_agent_location", 2, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);

		cout << "AgentLocation Result -> " << rval << ";" << endl;
	}
}

bool PrologHelper::AddUndestructableWall(int i, int j)
{
	if (i >= 20 || j >= 13)
		return false;
	if (g_initAi)
	{
		predicate_t pr = NULL;

		term_t t0 = PL_new_term_refs(3);

		int rval;

		PL_put_integer(t0, long(1));
		PL_put_integer(t0 + 1, i);
		PL_put_integer(t0 + 2, j);

		//if(!pr)
			pr = PL_predicate("add_wall", 3, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);

		cout << "AddUndestructableWall(" << i << ", " << j << ") -> " << rval << ";" << endl;

		return rval;
	}

	return true;
}

bool PrologHelper::AddDestructableWall(int i, int j)
{
	if (i >= 20 || j >= 13)
		return false;
	if (g_initAi)
	{
		predicate_t pr;

		term_t t0 = PL_new_term_refs(3);

		int rval;

		PL_put_integer(t0, long(2));
		PL_put_integer(t0+1, long(i));
		PL_put_integer(t0+2, long(j));

		pr = PL_predicate("add_wall", 3, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);

		cout << "AddDestructableWall Result(" << i << ", " << j << ") -> " << rval << ";" << endl;

		return rval;
	}

	return true;
}

void PrologHelper::AddClosedDoor(int i, int j)
{
	if (g_initAi)
	{
		predicate_t pr;

		term_t t0 = PL_new_term_refs(3);

		int rval;

		PL_put_integer(t0, -1);
		PL_put_integer(t0 + 1, i);
		PL_put_integer(t0 + 2, j);

		pr = PL_predicate("add_wall", 3, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);

//		cout << "AddClosedDoor Result -> " << rval << ";" << endl;
	}
}

void PrologHelper::SetBomb()
{
	//return;
	if (g_initAi)
	{
		int i, j;
		gameLayer->GetPleyerCoordinates(i, j);
		predicate_t pr;

		term_t t0 = PL_new_term_refs(3);

		int rval;

		term_value_t tp;

		PL_put_integer(t0 + 0, i);
		PL_put_integer(t0 + 1, j);

		pr = PL_predicate("add_bomb", 2, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);
		
		//cout << "Walk Result -> " << rval << "; " << tp.i << endl;

		//return tp.i;
	}
	//return 0;
}

int PrologHelper::Walk(int i, int j)
{
	int retwal = 0;
	if (g_initAi)
	{
		predicate_t pr;

		term_t t0 = PL_new_term_refs(3);

		int rval;

		term_value_t tp;

		PL_put_variable(t0 + 2);
		PL_put_integer(t0 + 0, i);
		PL_put_integer(t0 + 1, j);

		pr = PL_predicate("walk1", 3, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);

		PL_get_term_value(t0 + 2, &tp);

		cout << "Walk Result -> " << rval << "; " << tp.i << endl;

		retwal = tp.i;
	}
	if (g_initAi)
	{
		predicate_t pr;

		term_t t0 = PL_new_term_refs(3);

		int rval;

		term_value_t tp;

		PL_put_integer(t0 + 0, i);
		PL_put_integer(t0 + 1, j);

		pr = PL_predicate("set_bomb", 2, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);

		cout << "set_bomb ->" << rval << endl;
		
		if (rval)
			retwal = -retwal;
	}
	return retwal;
}

void PrologHelper::SetAgentLocation(int i, int j)
{

	if (g_initAi)
	{
		predicate_t pr;

		term_t t0 = PL_new_term_refs(3);

		int rval;

		term_value_t tp;

		PL_put_integer(t0 + 0, i);
		PL_put_integer(t0 + 1, j);

		pr = PL_predicate("set_agent_location", 2, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);
		
		cout << "set_agent_location -> " << rval  << endl;

		//return tp.i;
	}
	//return 0;
}

void PrologHelper::Explode(int i, int j)
{
	if (g_initAi)
	{
		predicate_t pr;

		term_t t0 = PL_new_term_refs(3);

		int rval;

		term_value_t tp;

		PL_put_integer(t0 + 0, i);
		PL_put_integer(t0 + 1, j);

		pr = PL_predicate("explode", 2, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);

		cout << "explode -> " << rval << endl;

		//return tp.i;
	}
}

void PrologHelper::DeleteWall(int i, int j)
{
	if (g_initAi)
	{
		predicate_t pr;

		term_t t0 = PL_new_term_refs(3);

		int rval;

		term_value_t tp;

		PL_put_integer(t0 + 0, i);
		PL_put_integer(t0 + 1, j);

		pr = PL_predicate("delete_wall", 2, NULL);

		rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);

		cout << "explode -> " << rval << endl;

		//return tp.i;
	}
	if (i == 5 && j == 5)
	{
	}
}

void PrologHelper::printWorld()
{
	ofstream f("C:\\Users\\Coder\\Desktop\\logW.txt");
	ofstream ff("C:\\Users\\Coder\\Desktop\\logC.txt");
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			{
				predicate_t pr;

				term_t t0 = PL_new_term_refs(3);

				int rval;

				term_value_t tp;

				PL_put_variable(t0 + 0);
				PL_put_integer(t0 + 1, i);
				PL_put_integer(t0 + 2, j);

				pr = PL_predicate("world", 3, NULL);

				rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);

				PL_get_term_value(t0 + 0, &tp);

				f << "world[" << i << ", " << j << "] -> " << tp.i << "  ans -> " << rval << endl;
			}
			{
				predicate_t pr;

				term_t t0 = PL_new_term_refs(3);

				int rval;

				term_value_t tp;

				PL_put_variable(t0 + 2);
				PL_put_integer(t0 + 0, i);
				PL_put_integer(t0 + 1, j);

				pr = PL_predicate("get_count1", 3, NULL);

				rval = PL_call_predicate(NULL, PL_Q_NORMAL, pr, t0);

				PL_get_term_value(t0 + 2, &tp);

				ff << "count[" << i << ", " << j << "] -> " << tp.i << "  ans -> " << rval << endl;
			}

		}
	}
	f.close();
	ff.close();
}

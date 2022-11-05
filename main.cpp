#include <iostream>
#include <random>
#include <time.h>
#include <algorithm>
#include <array>
#include <vector>

using namespace std;

void rewire_function(int WS[30][30], int rewired[30][30], double &p)
{
    for(int i = 0; i < 30; i++)
		for(int j = 0; j < 30; j++)
			rewired[i][j] = 0;
    double rand_p;
    for(int i = 0; i < 30; i++)
    {
        for(int j = i + 1; j < 30; j++)
        {
            // if have edge then rewired
            if(WS[i][j] == 1)
            {
                rand_p = (double)rand() / RAND_MAX;
                if(rand_p <= p)
                {
                    int a = 0;
                    int b = 0;
                    while(a == b)
                    {
                        a = rand() % 30;
                        b = rand() % 30;
                    }
                    rewired[a][b] = 1;
                    rewired[b][a] = 1;
                }
                else
                {
                rewired[i][j] = 1;
                rewired[j][i] = 1;
                }
            }
        }
    }
  /*  for(int i = 0; i < 30; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            cout << rewired[i][j] << " ";
        }
        cout << endl;
    } */
}

// this check function is nothing because WS model rewired could produce isolated node, if the node's k is bigger than 1,
// this game is invalid
/*
bool check_Multi_Domination(int rewired[30][30], int player_strategy[30], int k[30])
{
    for(int i = 0; i < 30; i++)
    {
        int domination = 0;

        // pi in the set
        if(player_strategy[i] == 1)
        {
            domination++;
            for(int j = 0; j < 30; j++)
            {
                if(rewired[i][j] == 1)
                {
                    if(player_strategy[j] == 1)
                        domination++;
                }
            }
            if(domination < k[i])
                return false;
        }

        // pi out the set
        else
        {
            for(int j = 0; j < 30; j++)
            {
                if(rewired[i][j] == 1)
                {
                    if(player_strategy[j] == 1)
                        domination++;
                }
            }
            if(domination < k[i])
                return false;
        }
    }
    return true;
}
*/


void Multi_Domination_game(int rewired[30][30], int &cardinality, int &move_count)
{
    cardinality = 0;
    move_count = 0;
    bool NE = false;
    int player_state[30] = {0}; // each player determine its strategy in this loop, no = 0, yes = 1
    int player_strategy[30] = {0}; // out = 0 , in = 1
    int k[30] = {0};
    int pi;

    for(int i = 0; i < 30; i++)
    {
        // kj be a random integer within the range [1,3]
        k[i] = rand() % 3 + 1;


        // randomize initial game state, out = 0, in = 1
        player_strategy[i] = rand() % 2;
    }

    while(NE != true)
    {
        // random pick up one player who can improve its utility
        pi = rand() % 30;
        while(player_state[pi] == 1)
        {
            pi = rand() % 30;
        }
        player_state[pi] = 1;
     // count now pi have which dominations int his closed neighbors
        int nums_domination = 0;
        for(int i = 0; i < 30; i++)
        {
            if(pi == i)
            {
                if(player_strategy[pi] == 1)
                    nums_domination++;
                continue;
            }
            if(rewired[pi][i] == 1)
            {
                if(player_strategy[i] == 1)
                    nums_domination++;
            }
        }
        
        // pi in the set
        if (player_strategy[pi] == 1)
        {
            // dominations > k[pi] => pi is redundant
            if(nums_domination > k[pi])
            {
                player_strategy[pi] = 0;
                move_count++;

                //because pi change the strategy so all player decide in or out
                for(int i = 0; i < 30; i++)
                    player_state[i] = 0;
                player_state[pi] = 1;
                continue;
            }
        }

        // pi out the set
        else
        {
            if(nums_domination < k[pi])
            {
                player_strategy[pi] = 1;
                move_count++;
                //because pi change the strategy so all player decide in or out
                for(int i = 0; i < 30; i++)
                    player_state[i] = 0;
                player_state[pi] = 1;
                continue;
            }
        }


        // test the all player never change their strategy
        int flag = 0;
        for(int i = 0; i < 30; i++)
        {
            if(player_state[i] == 0)
                flag = 1;
        }

        if(flag == 0)
            NE = true;
    }
    for(int i = 0; i < 30; i++)
    {
        if(player_strategy[i] == 1)
            cardinality++;
    }

    // check game state valid or invalid
    /*
    bool check_MD = check_Multi_Domination(rewired, player_strategy, k);

    if(check_MD == false)
        cout << "this game state is invalid" << endl;
    */
}

bool check_Symmetric_MDS_based_IDS(int rewired[30][30], int player_strategy[30])
{
    for(int i = 0; i < 30; i++)
    {
        // pi in the set
        if(player_strategy[i] == 1)
        {
            for(int j = 0; j < 30; j++)
            {
                if(rewired[i][j] == 1)
                {
                    if(player_strategy[j] == 1)
                        return false;
                }
            }
        }

        // pi out the set
        else
        {
            int flag = 0;
            for(int j = 0; j < 30; j++)
            {
                if(rewired[i][j] == 1)
                {
                    if(player_strategy[j] == 1)
                        flag = 1;
                }
            }
            if(flag == 0)
                return false;
        }
    }
    return true;
}


void Symmetric_MDS_based_IDS_game(int rewired[30][30], int &cardinality, int &move_count)
{
    cardinality = 0;
    move_count = 0;
    bool NE = false;
    int player_state[30] = {0}; // each player determine its strategy in this loop, no = 0, yes = 1
    int player_strategy[30] = {0}; // out = 0 , in = 1
    int pi;

    for(int i = 0; i < 30; i++)
    {
        // randomize initial game state, out = 0, in = 1
        player_strategy[i] = rand() % 2;
    }

    while(NE != true)
    {
        // random pick up one player who can improve its utility
        pi = rand() % 30;
        while(player_state[pi] == 1)
        {
            pi = rand() % 30;
        }
        player_state[pi] = 1;

        // if pi out of set
        if(player_strategy[pi] == 0)
        {
            int neighbor_in_set = 0;
            for(int i = 0; i < 30; i++)
            {
                // check the neighbor of pi
                if(rewired[pi][i] == 1)
                {
                    // if the neighbor of pi in the set then pi need not to join the set
                    if(player_strategy[i] == 1)
                    {
                        neighbor_in_set = 1;
                        break;
                    }
                }
            }

            // if all neighbor of pi are not in the set then pi join the set
            if(neighbor_in_set == 0)
            {
                player_strategy[pi] = 1;
                move_count++;
                //because pi change the strategy so all player decide in or out
                for(int i = 0; i < 30; i++)
                    player_state[i] = 0;
                player_state[pi] = 1;
                continue;
            }
        }

        // if pi in the set -> player_strategy[pi] = 1
        else
        {
            int neighbor_in_set = 0;
            for(int i = 0; i < 30; i++)
            {
                // check the neighbor of pi
                if(rewired[pi][i] == 1)
                {
                    // so pi need not to join the set
                    if(player_strategy[i] == 1)
                    {
                        neighbor_in_set = 1;
                        break;
                    }
                }
            }
            if(neighbor_in_set == 1)
            {
                player_strategy[pi] = 0;
                move_count++;
                //because pi change the strategy so all player decide in or out
                for(int i = 0; i < 30; i++)
                    player_state[i] = 0;
                player_state[pi] = 1;
                continue;
            }
        }

        // test the all player never change their strategy
        int test_NE = 0;
        for(int i = 0; i < 30; i++)
        {
            if(player_state[i] == 0)
                test_NE = 1;
        }
        if(test_NE == 0)
            NE = true;
    }

    for(int i = 0; i < 30; i++)
    {
        if(player_strategy[i] == 1)
            cardinality++;
    }

    // check game state valid or invalid
    
    bool check_S_MDS_based_IDS = check_Symmetric_MDS_based_IDS(rewired, player_strategy);

    if(check_S_MDS_based_IDS == false)
        cout << "this game state is invalid" << endl;
    

}

bool check_Maximal_Matching(int rewired[30][30], int matching[30])
{
    for(int i = 0; i < 30; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            if(rewired[i][j] == 1)
            {
                if(matching[i] == j && matching[j] == i)
                {
                    for(int k = 0; k < 30; k++)
                    {
                        if(k == i || k == j)
                            continue;
                        else
                            if(matching[k] == i && matching[k] == j)
                                return false;
                    }
                }
            }
        }
    }
    return true;
}

void Maximal_Matching(int rewired[30][30], int &pair, int &move_count)
{
    //lower degree have higher priority

    int degree[30] = {};
    move_count = 0;
    pair = 0;
    bool NE = false;
    int pi;
    int player_state[30] = {0}; // each player determine its strategy in this loop, no = 0, yes = 1
    int player_strategy[30] = {0}; // out = 0 , in = 1


    int edge = 0;
    // calculate the degree of the node and the nums of edge
    for(int i = 0; i < 30; i++)
    {
        for(int j = 0; j < 30; j++)
        {
            if(rewired[i][j] == 1)
            {
                degree[i]++;
                edge++;
            }
        }
    }
    edge = edge / 2;

    //record the matching of node, init the node not in the matching so they are -1
    int matching[30];
    for(int i = 0; i < 30; i++)
        matching[i] = -1;

    // randomize the inital state, the state also a matching
    int left, right;
    while(edge > 0)
    {
        left = rand() % 30;
        right = rand() % 30;

        //random select two node which need to connect
        while(left == right || rewired[left][right] == 0)
        {
            left = rand() % 30;
            right = rand() % 30;
        }

        // two node need to not in the matching and rand need to = 1   -> then the edge can join to the init matching
        if(matching[left] != -1 || matching[right] != -1 || rand() % 2 == 0)
        {
            edge--;
            continue;
        }

        // the edge join to the init matching
        matching[left] = right;
        matching[right] = left;
        edge--;
    }

    while(!NE)
    {
        // random pick up one player who can improve its utility
        pi = rand() % 30;
        while (player_state[pi] == 1) 
        {
            pi = rand() % 30;
        }
        player_state[pi] = 1;

        // pi in the matching but pi can choose the lower degree neighbor which not in the matching and pair it
        if(matching[pi] > -1)
        {
            int min_degree = degree[matching[pi]];
            int select_node = matching[pi];  //original pair
            bool choose = false;
            for(int i = 0; i < 30; i++)
            {
                if(rewired[pi][i] == 1)
                {
                    if(degree[i] < min_degree && matching[i] == -1)
                    {
                        min_degree = degree[i];
                        select_node = i;
                        choose = true;
                    }
                }
            }
            matching[pi] = select_node;
            matching[select_node] = pi;
            if(choose)
            {
                move_count++;
                for (int i = 0; i < 30; i++)
                    player_state[i] = 0;
                player_state[pi] = 1;
            }    
        }

        //matching[pi] = -1 , pi not in the matching so pi can choose the min degree node which not in the matching and pair it
        else
        {
            int min_degree = 100;
            int select_node;
            bool choose = false;
            for(int i = 0; i < 30; i++)
            {
                if(rewired[pi][i] == 1)
                {
                    if(degree[i] < min_degree && matching[i] == -1)
                    {
                        min_degree = degree[i];
                        select_node = i;
                        choose = true;
                    }
                }
            }
            if(choose)
            {
                matching[pi] = select_node;
                matching[select_node] = pi;
                move_count++;
                for (int i = 0; i < 30; i++)
                    player_state[i] = 0;
                player_state[pi] = 1;
            }
        }
        
        // test the all player never change their strategy
        int flag = 0;
        for (int i = 0; i < 30; i++) 
        {
            if (player_state[i] == 0)
                flag = 1;
        }

        if (flag == 0)
            NE = true;
    }

    for(int i = 0; i < 30; i++)
        {
            if(matching[i] != -1)
                pair++;
        }
        pair = pair / 2;


    // check game state valid or invalid
    
    bool check_Max_Matching = check_Maximal_Matching(rewired, matching);

    if(check_Max_Matching == false)
        cout << "this game state is invalid" << endl;
}

int main(void)
{
    srand(time(NULL));
    int n = 30, k = 4;
    double p;
    int WS[30][30] = {0};
    int rewired[30][30] = {0};

    for(int i = 0; i < n; i ++)
    {
        if(i > 1 && i < 28)
        {
            WS[i][i-1] = 1;
            WS[i][i-2] = 1;
            WS[i][i+1] = 1;
            WS[i][i+2] = 1;
        }
        else
        {
            if(i == 0)
            {
                WS[0][28] = 1;
                WS[0][29] = 1;
                WS[0][1] = 1;
                WS[0][2] = 1;
            }
            if(i == 1)
            {
                WS[1][0] = 1;
                WS[1][29] = 1;
                WS[1][2] = 1;
                WS[1][3] = 1;
            }
            if(i == 28)
            {
                WS[28][27] = 1;
                WS[28][26] = 1;
                WS[28][29] = 1;
                WS[28][0] = 1;
            }
            if(i == 29)
            {
                WS[29][27] = 1;
                WS[29][28] = 1;
                WS[29][0] = 1;
                WS[29][1] = 1;
            }
        }
    }


  /*  int move_count, cardinality;
    rewire_function(WS, rewired, p);
    Multi_Domination_game(rewired, cardinality, move_count);
    cout << "cardinality = "<< cardinality << endl << "move_count = "<< move_count << endl;
*/
    int move_count, cardinality, pair;
    int cardinality_Multi_Domination;
    int move_count_Multi_Domination;
    int cardinality_Symmetric_MDS_based_IDS_game;
    int move_count_Symmetric_MDS_based_IDS_game;

    int pair_Maximal_matching;
    int move_count_Maximal_matching;

    float cardinality_Multi_Domination_avg = 0;
    float move_count_Multi_Domination_avg = 0;
    float cardinality_Symmetric_MDS_based_IDS_game_avg = 0;
    float move_count_Symmetric_MDS_based_IDS_game_avg = 0;
    float pair_Maximal_matching_avg = 0;
    float move_count_Maximal_matching_avg = 0;

    int nums_case = 100;

  //  Symmetric_MDS_based_IDS_game(rewired, cardinality, move_count);
    for(p = 0; p < 1; p += 0.2)
    {
        cardinality_Multi_Domination = 0;
        move_count_Multi_Domination = 0;
        cardinality_Symmetric_MDS_based_IDS_game = 0;
        move_count_Symmetric_MDS_based_IDS_game = 0;
        pair_Maximal_matching = 0;
        move_count_Maximal_matching = 0;

        for(int i = 0; i < nums_case; i++)
        {
            rewire_function(WS, rewired, p);
            Multi_Domination_game(rewired, cardinality, move_count);
          //  cout << "cardinality = "<< cardinality << " " << "move_count = "<< move_count << endl;
            cardinality_Multi_Domination += cardinality;
            move_count_Multi_Domination += move_count;

            Symmetric_MDS_based_IDS_game(rewired, cardinality, move_count);
            cardinality_Symmetric_MDS_based_IDS_game += cardinality;
            move_count_Symmetric_MDS_based_IDS_game += move_count; 

            Maximal_Matching(rewired, pair, move_count);
            pair_Maximal_matching += pair;
            move_count_Maximal_matching += move_count;
        }
        for(int i = 0; i < nums_case; i++)

        cardinality_Multi_Domination_avg = (float) cardinality_Multi_Domination / nums_case;
        move_count_Multi_Domination_avg = (float) move_count_Multi_Domination / (nums_case * 30);

        cardinality_Symmetric_MDS_based_IDS_game_avg = (float) cardinality_Symmetric_MDS_based_IDS_game / nums_case;
        move_count_Symmetric_MDS_based_IDS_game_avg = (float) move_count_Symmetric_MDS_based_IDS_game / (nums_case * 30);

        pair_Maximal_matching_avg = (float) pair_Maximal_matching / nums_case;
        move_count_Maximal_matching_avg = (float) move_count_Maximal_matching / (nums_case * 30);

        cout << "now p = " << p << endl;
        cout << "Requirement1-1,"<< " the avg of cardinality = " << cardinality_Multi_Domination_avg << ", and the avg of move_count = " << move_count_Multi_Domination_avg << endl;
        cout << "Requirement1-2,"<< " the avg of cardinality = " << cardinality_Symmetric_MDS_based_IDS_game_avg << ", and the avg of move_count = " << move_count_Symmetric_MDS_based_IDS_game_avg << endl;
        cout << "Requirement2," << " the avg of pair = " << pair_Maximal_matching_avg << ", and the avg of move_count = " << move_count_Maximal_matching_avg << endl;  
    }
    


    cout << "------------------------------------------------------" << endl;

    return 0;
}
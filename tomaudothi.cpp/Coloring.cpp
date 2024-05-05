#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <map>
#include <deque>
#include <list>
using namespace std;

string test = "test2";
string path = "/Users/ACER/Coloring/Input/" + test + ".txt"; // file input
vector<vector<int>> adjacency_List(10000); // danh sách kề chứa đồ thị
vector<vector<int>> adjacency(10000,vector<int>(10000));
int nodes, edge; // lần lượt là số đỉnh và cạnh
int tabu_size = 7, reps = 10000, max_iterations = 10000; //tăng những thông số này sẽ tăng khả năng tìm được sắc số tốt hơn nhưng sẽ tăng tốc độ chương trình
//đọc các nghiên cứu khoa học thì kích cỡ của tabu_size được đề xuất là nằm trong từ 5-12
bool check_status = true;

string time ()
{
    auto current_time = chrono::system_clock::now();
    // Chuyển đổi thời điểm hiện tại sang dạng chuỗi
    time_t current_time_t = chrono::system_clock::to_time_t(current_time);
    string current_time_str = ctime(&current_time_t);
    return current_time_str;
}
void read_graph() // hàm đọc file input vào một danh sách kề để tính toán
{
    ifstream in;
    in.open(path);
    in >> nodes >> edge;
    for (int i = 0; i < edge; i++)
    {
        int a, b;
        in >> a >> b;
        adjacency_List[a - 1].push_back(b - 1);
        adjacency_List[b - 1].push_back(a - 1);
        adjacency[a - 1][b - 1] = 1;
        adjacency[b - 1][a - 1] = 1;
    }
    in.close();
}

void print_result(vector<int> solution) // in ra đáp án ĐANG chấp nhận được để không cần chạy hết thuật toán
{
    // In ra thời điểm hiện tại cùng màu tìm được để kiểm tra tốc độ chạy
    cout << "Found color : " << *max_element(solution.begin(), solution.end()) + 1 <<",time : " << time()<< endl;
    ofstream out;
    out.open("/Users/ACER/Coloring/Output/"+test + "tomau.txt"); // cần thiết thì thêm "/Users/ACER/Coloring/Input/"
    out << *max_element(solution.begin(), solution.end()) + 1 << endl;
    vector <int> temp1(10000);
    for (int i = 0; i < solution.size(); i++)
    {
        out << solution[i] + 1 << endl;
        temp1[solution[i]]++;
    }
    out.close(); 
}


int dsatur(int vertex, vector<int>& result) // tính toán số lượng màu khác nhau được sử dụng bởi các đỉnh kề của vertex.
{
    set<int> colors; // dùng để lưu trữ các màu đã sài của các đỉnh kề với đỉnh vertex
    for (int i = 0; i < adjacency_List[vertex].size(); i++)
    {
        if (result[adjacency_List[vertex][i]] != -1) // tìm màu của các đỉnh kề với đỉnh vertex đã được tô màu
        {
            colors.insert(result[adjacency_List[vertex][i]]); //thêm màu đó vào biến colors
        }
    }
    return colors.size(); // trả về kích cỡ của biến colors hay chính là số màu đã được sử dụng bởi các đỉnh kề với đỉnh vertex
}

vector<int> greedy_coloring() //dùng thuật dsatur để tính toán màu ban đầu
{
    int max_degree_node = 0; //biến chứa đỉnh có bậc cao nhất
    int max_degree = 0; // biến chứa bậc cao nhất
    //tìm bậc lớn nhất trong đồ thị 
    for (int i = 0; i < nodes; i++)
    {
        int degree = adjacency_List[i].size();
        if (degree > max_degree)
        {
            max_degree = degree;
            max_degree_node = i;
        }
    }

    vector<int> result(nodes, -1); //chứa đáp án của thuật dsatur, ban đầu khai báo tất cả phần tử là -1
    result[max_degree_node] = 0; //để đỉnh có bậc cao nhất tô màu 0
    set<int> uncolored_vertices; // chứa các đỉnh chưa được tô màu
    // thêm các đỉnh chưa được tô màu vào uncolored_vertices
    for (int i = 0; i < nodes; i++)
    {
        if (i != max_degree_node)
        {
            uncolored_vertices.insert(i);
        }
    }

    while (!uncolored_vertices.empty()) // dừng khi tất cả các đỉnh đã được tô màu
    {
        int max_dsatur = -1;
        int next_vertex = -1;
        // tìm đỉnh tiếp theo để tô màu dựa theo khái niệm "mức độ bão hòa"
        // mức độ bão hòa được định nghĩa là số màu khác nhau được sử dụng trong các đỉnh kề với đỉnh được xét
        for (auto u : uncolored_vertices) 
        {
            int sat = dsatur(u,result); 
            if (sat > max_dsatur)
            {
                max_dsatur = sat;
                next_vertex = u;
            }
            /*else if (sat == max_dsatur)
            {
                if (adjacency_List[u].size() >= adjacency_List[next_vertex].size()) next_vertex = u; //////////////////////////////////////////////////////////////
            } */
        }

        int min_avail_color = 0;
        bool check = true;
        // tìm màu nhỏ nhất chưa được sử dụng trong số các đỉnh kề với next_vertex
        for (int i = 0; i < adjacency_List[next_vertex].size(); i++)
        {
            if (result[adjacency_List[next_vertex][i]] == min_avail_color)
            {
                min_avail_color++;
                i = -1; 
            }
        }
        result[next_vertex] = min_avail_color; // tô đỉnh tiếp theo với màu đã chọn
        uncolored_vertices.erase(next_vertex); // xóa đỉnh đã được tô khỏi danh sách các đỉnh chưa được tô màu
    }

    return result;
}
vector<int> greedy_coloring2() 
{
    vector<int> result(nodes, -1);
    vector<int> degree(nodes,0);
    vector<int> sort_by_degree(nodes);
    for (int i = 0; i < nodes;i++) 
    {
        degree[i] = adjacency_List[i].size();
        sort_by_degree[i] = i;
    }
    for (int i = 0; i < nodes;i++)
    {
        for (int j = i + 1; j < nodes - 1;j++)
        {
            if (degree[j] < degree[j + 1]) swap(sort_by_degree[j + 1],sort_by_degree[j]);
        }
    }
    result[sort_by_degree[0]] = 0;
    //result[0] = 0;
    for (int i = 1; i < nodes;i++)
    {
        int vertice = sort_by_degree[i];
        //int vertice = i;
        int color = 0;
        vector<bool> available(nodes,true);
        for (int j = 0; j < adjacency_List[vertice].size();j++) if (result[adjacency_List[vertice][j]] != -1) available[result[adjacency_List[vertice][j]]] = false;
        for (color = 0; color < nodes;color++) 
        {
            if (available[color] == true) break;
        }
        result[vertice] = color;
    }  
    return result;
}
int binarySearch(vector<int> arr, int l, int r, int x) //sử dụng để tối ưu khả năng tìm kiếm một phần tử trong adjacency_List, trả về vị trí của phần tử đó
{
	while (l <= r) {
		int m = l + (r - l) / 2;
		if (arr[m] == x)
			return m;
		if (arr[m] < x)
			l = m + 1;
		else
			r = m - 1;
	}
	return -1; // nếu không tìm thấy phần tử đó thì trả về -1
}
// nhiệm vụ của hàm là tìm cách tô với number_of_colors màu
vector<int> tabu_coloring(int number_of_colors,vector<int> previous_solution,bool is_first_solution)
{
    srand(time(NULL));
    vector<int> colors(number_of_colors); // chứa các màu chạy từ 0 tới number_of_colors - 1, tức là màu nhỏ hơn màu tìm được ở đáp án cũ
    int iterations = 0; // biến điếm số lần lặp
    list <pair<int, int>> tabu_list; // list chứa cặp đỉnh màu đã được chọn để đổi màu trước đó và không được phép đặt lại trong tabu_size bước
    map<int, int> aspiration_dict; // lưu trữ mức aspiration cho từng mức xung đột,số lượng xung đột mới thấp hơn hoặc bằng mức aspiration, giải pháp mới được coi là tốt hơn hiện tại
    vector<int> solution(previous_solution.size()); // lưu trữ một giải pháp tạm thời
    // thêm vào solution các dữ liệu từ lời giải trước (previous_solution)
    for (int i = 0; i < previous_solution.size(); ++i)
    {
        solution[i] = previous_solution[i];
    }
    if (is_first_solution == false) // kiểm tra xem đây có phải là lần đầu tiên thuật toán tìm kiếm Tabu được thực hiện tìm kiếm không? Nói cách khác thì là kiểm tra xem phải là kết quả từ hàm dsatur ban đầu không
    {
        // với những màu lớn hơn hoặc bằng số màu đang muốn hướng tới thì tô nó bằng một màu random nào đó trong kho màu colors
        for (int i = 0; i < solution.size(); i++)
        {
            if (solution[i] >= number_of_colors) solution[i] = rand() % colors.size();
        }
    }
    int conflict_count = 0; // biến tính số xung đột màu
    int first = true;
    vector <int> conflict_nodes;
    int changed_node;
    while(iterations < max_iterations) // biến while này chạy theo biến max_iterations đã được set-up ở trên, tùy theo độ lớn của biến max_iterations mà chất lượng màu sẽ thay đổi
    {
        conflict_count = 0;
        list <int> listofcandidates; // lưu trữ các đỉnh có xung đột màu trong quá trình tìm kiếm một giải pháp màu cho đồ thị
        // tính toán số lượng xung đột của cách tô màu hiện tại
        if (first) // done
        {
            vector <vector<int>> colorteam(number_of_colors);  
            for (int i = 0; i < nodes;i++)
            {
                colorteam[solution[i]].push_back(i);
            }
            for (int i = 0; i < number_of_colors;i++)  
            {
                for (int j = 0; j < colorteam[i].size();j++) 
                {
                    for (int k = j + 1; k < colorteam[i].size();k++)
                    {
                        if (adjacency[colorteam[i][j]][colorteam[i][k]] == 1)
                        {
                            conflict_count++;
                            listofcandidates.push_back(colorteam[i][j]);
                            listofcandidates.push_back(colorteam[i][k]);
                        }
                    }
                }
            }
        }
        else 
        {
            for (int i = 0; i < conflict_nodes.size();i++)
            {
                for (int j = i + 1; j < conflict_nodes.size();j++)
                {
                    if (solution[conflict_nodes[i]] == solution[conflict_nodes[j]] && adjacency[conflict_nodes[i]][conflict_nodes[j]] == 1)
                    {
                        conflict_count++;
                        listofcandidates.push_back(conflict_nodes[i]);
                        listofcandidates.push_back(conflict_nodes[j]);
                    }
                }
            }
            for (int i = 0; i < adjacency_List[changed_node].size();i++)
            {
                if (solution[adjacency_List[changed_node][i]] == solution[changed_node]) 
                {
                    conflict_count++;
                    listofcandidates.push_back(changed_node);
                    listofcandidates.push_back(adjacency_List[changed_node][i]);
                }
            }
        }
        set <int> candidates(listofcandidates.begin(),listofcandidates.end()); //chuyển listofcandidates từ list sang set để lọc bớt các phần tử trùng lặp
        vector<int> candidatesvector(candidates.begin(), candidates.end()); // chuyển candidates từ set thành vector cho dễ truy cập
        conflict_nodes = candidatesvector;
        int vertice;
        if (conflict_count == 0) break; // nếu số xung đột bằng 0 tức là cách tô màu hợp lệ và dừng lại để in ra màu đó
        vector<int> new_solution(solution.size()); // chứa cách tô màu mới
        int new_conflicts = 0,old_vertice_conflicts = 0;
        for (int i = 0; i < reps;i++)
        {
            // lấy random màu và đỉnh xung đột từ candidates và colors
            vertice = candidatesvector[rand() % candidates.size()];
            int new_color = rand() % colors.size();
            // tìm một màu random mới cho đỉnh mới 
            while (solution[vertice] == new_color) new_color = rand() % colors.size();
            // tạo new_solution để tính toán độc lập với solution và tìm cách tô màu mới
            for (int h = 0; h < solution.size();h++) new_solution[h] = solution[h];
            // đặt đỉnh đã chọn thành màu mới
            new_solution[vertice] = new_color;
			new_conflicts = 0,old_vertice_conflicts = 0;
			// tính toán lượng xung đột mới trong cách tô màu mới
            // được tối ưu từ O(n^2) thành O(n) nhờ sử dụng lại 1 phần kết quả đã được tính toán từ trước
            for (int j = 0; j < adjacency_List[vertice].size();j++)
            {
                if (new_solution[vertice] == new_solution[adjacency_List[vertice][j]]) new_conflicts += 1; 
                if (solution[vertice] == solution[adjacency_List[vertice][j]]) old_vertice_conflicts += 1;
            }
            new_conflicts = conflict_count +  new_conflicts - old_vertice_conflicts; 
            // so sánh lại xung đột từ cách tô màu cũ và mới
            if (new_conflicts < conflict_count)
            {
                // Kiểm tra và cập nhật giá trị trong aspiration_dict
                if (new_conflicts <= aspiration_dict[conflict_count]) 
                {
                    aspiration_dict[conflict_count] = new_conflicts - 1;
                    // nếu số xung đột mới tìm được nhỏ hơn aspiration thì nó sẽ được cập nhật lại aspiration
                    // Tìm để xóa cặp đỉnh và màu trong tabu_list
                    for (auto it = tabu_list.begin(); it != tabu_list.end(); ++it)
                    {
                        if (*it == make_pair(vertice, new_color))
                        {
                            tabu_list.erase(it);
                            break;
                        }
                    }
                }
                else
                {
                    // Kiểm tra cặp đỉnh và màu có trong tabu_list hay không
                    bool found = false;
                    for (const auto& pair : tabu_list)
                    {
                        if (pair == make_pair(vertice, new_color))
                        {
                            found = true;
                            break;
                        }
                    }
                    // nếu tìm thấy thì tiếp tục chạy tiếp
                    if (found)
                    {
                        continue;
                    }
                }
                // nếu không tìm thấy thì dừng lại
                break;
            } 
        } 
        tabu_list.push_back(make_pair(vertice, solution[vertice])); // thêm cặp đỉnh và màu vừa được tô vào danh sách tabu_list để lần sau không thể chọn lại
        if (tabu_list.size() > tabu_size) tabu_list.pop_front(); // xóa cặp đỉnh và màu cuối, nói cách khác là cặp đã lâu chưa được sử dụng, giờ cho phép sử dụng lại
        for (int i = 0; i < solution.size();i++) if (solution[i] != new_solution[i]) {changed_node = i; break;}
        solution = new_solution; // cập nhật đáp án mới 
        first = false;
        iterations += 1; // tăng biến iterations để dừng chương trình
    }

    if (conflict_count != 0) // nếu xung đột màu khắc 0 tức là cách tô màu sau nhiều lần tìm kiếm đều không tìm được cách tô hợp lệ
    {
        auto current_time = chrono::system_clock::now();
        // Chuyển đổi thời điểm hiện tại sang dạng chuỗi
        time_t current_time_t = chrono::system_clock::to_time_t(current_time);
        string current_time_str = ctime(&current_time_t);
        cout << "No coloring found with " << number_of_colors << " color,time : "<< current_time_str << endl; // in ra thông báo không tìm được cách tô màu hợp lệ
        check_status = false;
        return previous_solution; // trả về cách tô màu cũ
    }
    else // nếu tìm được cách tô hợp lệ thì in ra đáp án mới vào file output và trả về đáp án mới
    {
        print_result(solution);
        return solution; // trả về cách tô màu mới tối ưu hơn để tiếp tục tính toán tiếp
    }
    return solution;
}

// giảm số màu tô sau mỗi lần chạy để tìm cách tô màu nhỏ nhất có thể
void tabu_search(vector <int>greedy_result_dict, int greedy_result_number)
{
    bool first_coloring = true; // biến để kiểm tra xem có phải lần tô đầu tiên từ đáp án của dsatur hay không
    vector<int>result = greedy_result_dict; // lấy dữ liệu từ đáp án từ dsatur
    for (int i = greedy_result_number + 1;i > 1;i -= 1)
    {
        result = tabu_coloring(i, result, first_coloring); // chạy thuật toán tìm cách tô màu với màu i rồi,cập nhật kết quả mới
        if (check_status == false) break; // nếu không tìm được cách tô với màu i thì dừng lại
        first_coloring = false;
    }
}
// đọc lại dữ liệu từ file output
void continue_tabu(vector<int> &greedy_result)
{
    ifstream in;
    in.open("/Users/ACER/Coloring/Output/" + test + "tomau.txt");
    int colors,temp = 0, i = 0;
    in >> colors;
    while (i < nodes)
    {
        in >> temp;
        greedy_result.push_back(temp - 1);
        i++;
    }
    in.close();
}
int main()
{
    cout << path <<",time : "<<time() << endl;
    read_graph(); // đọc file input
    vector<int> greedy_result;
    //greedy_result = greedy_coloring(); // chạy thuật toán dsatur để tìm đáp án đầu tiên
    //print_result(greedy_result);
    continue_tabu(greedy_result); // đọc lại dữ liệu từ file output đã được ghi trước đó, sử dụng lại dữ liệu để tiếp tục tính toán tiếp trong trường hợp gặp sự cố
    tabu_search(greedy_result,*max_element(greedy_result.begin(), greedy_result.end())); // chạy thuật toán tabu_search để tìm kiếm sắc số tốt hơn
    return 0;
}
#include <iostream>
#include <vector>

using namespace std;

struct seznam{
    long long base;
    long long current;
};
        
int main(){
    long long n, m;
    cin >> n >> m;
    
    long long a[n];
    vector <struct seznam> q;

    for(long long i = 0; i < n; i++){
        long long num;
        cin >> num;
        q.push_back({num, num});
    }

    for (long long i = 0; i < m; i++){
        cin >> a[i]; 
    }
    if(q[0].base == 1){
        for(int i = 0; i < m; i++)
            cout << a[i] << endl;
        return 0;
    }   

    long long j = 0;
    for(long long i = 1; i <= a[m-1]; i++){
        
        if(a[j] == i){
            cout << q[0].current << endl;
            j++;
            if(j > m)
                return 0;
        }

        q[0].current += q[0].base;
        long long k = 1;

        while(q[k-1].current > q[k].current && k < q.size()){
            swap(q[k-1], q[k]);
            k++;
        }

        while(q[k-1].current == q[k].current){
            q[k].current += q[k].base;
            k++;
            while(q[k-1].current > q[k].current && k < q.size()){
                swap(q[k-1], q[k]);
                k++;
            }
        }
    }

    return 0;
}

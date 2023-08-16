#include<cstdio>
#include<cstdint>
#include<vector>
#include<algorithm>

using namespace std;

const int max_cidr_subrange = 6;

struct cidr{
	uint32_t value;
	uint32_t mask;

	static uint32_t get_mask(int l);
	static int get_mask_l(uint32_t mask);

	void read(FILE* f);
	void print(FILE* f);
	void reduce(uint32_t new_mask);
	int ip_count()const;

	bool operator==(const cidr& another)const{
		return value == another.value && mask == another.mask;
	}
	bool operator<(const cidr& another)const{
		return value < another.value;
	}
};

int cidr::ip_count()const{
	int l = get_mask_l(mask);
	return 1<<(32-l);
}

int cidr::get_mask_l(uint32_t mask){
	int l;
	for(l = 0; mask; l++)
		mask<<=1;
	return l;
}

uint32_t cidr::get_mask(int l){
	uint32_t m = (int)((1ll<<l)-1);
	while(!(m& (1<<31)))
		m<<= 1;
	return m;
}

void cidr::read(FILE* f){
	int a, b, c, d, l;
	fscanf(f, "%d.%d.%d.%d/%d\n", &a, &b, &c, &d, &l);
	value = (a<<24) | (b<<16) | (c<<8) | d;
	mask = get_mask(l);
}

void cidr::print(FILE* f){
	int a, b, c, d, l;
	a = (value >> 24) & 255;
	b = (value >> 16) & 255;
	c = (value >> 8) & 255;
	d = (value) & 255;
	l = get_mask_l(mask);
	fprintf(f, "%d.%d.%d.%d/%d\n", a, b, c, d, l);
}

void cidr::reduce(uint32_t new_mask){
	mask = new_mask;
	value &= mask;
}

cidr merge(cidr a, cidr b){
	for(int ml = min(cidr::get_mask_l(a.mask), cidr::get_mask_l(b.mask)); ml >= max_cidr_subrange; ml--){
		auto mask = cidr::get_mask(ml);
		a.reduce(mask); b.reduce(mask);
		if(a == b)
			return a;
	}
	a.value = 0; a.mask = 0;
	return a;
}

vector<cidr> merge_cidrs(vector<cidr> s){
	bool merged = false;
	int i;
	vector<cidr> result;
	for(i = 1; i < (int)s.size(); i++){
		if(merge(s[i-1], s[i]).value){
			result.push_back(merge(s[i-1], s[i]));
			i++;
			merged = true;
			if(i == (int)s.size()-1)
				result.push_back(s[i]);
		}else{
			result.push_back(s[i-1]);
			if(i == (int)s.size()-1)
				result.push_back(s[i]);
		}
	}
	return merged? merge_cidrs(result): s;
}

vector<cidr> read_cidrs(FILE* f){
	vector<cidr> result;
	while(!feof(f)){
		cidr a;
		a.read(f);
		result.push_back(a);
	}
	return result;
}

int main(){
	auto s = read_cidrs(stdin);
	double total_ip = UINT32_MAX;
	double allow_ip = 0;
	stable_sort(s.begin(), s.end());
	s = merge_cidrs(s);
	for(auto& item: s){
		item.print(stdout);
		allow_ip += item.ip_count();
	}
	printf("%.2lf%% IP Allowed\n", allow_ip/total_ip*100.0);
}

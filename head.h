#ifndef listkmp_h
#define listkmp_h


// test1

template<class M> class Listext
{
public:
    Listext(int size = 20);
    M &operator[](int index);
    int getlength(void);
    M getitem(int index);
    void append(T *person_info);
    void insert(int index, T *person_info);
    void remove(int index);
    void concat(Listext<M> &list);
    bool member(T *person_info);
    bool equal(Listext<M> &list);
    bool sublist(Listext<M> &list);
    void prettyprint(void);
    void rotate(int m);
    M select(int k);
private:
    int numitems;
    M *reprarray;
    int maxsize, minssize;
    void allocate(void);
    void deallocate(void);
};

template<class F> class Fiheap
{

}


template<class T> class person_info: public Listext
{
    public:
    void input();
    void update();
    
    private:
    int ID;             //身份证号18位
    char name[];        //姓名拼写 ZHANGSAN  姓加名
    int address[];     //坐标（x，y）
    int phone[];        //11位手机号
    char wechat[];      //花里胡哨
    char email[];       //花里胡哨，too
    int job_grade;      //0-6，0 first
    int birth[];        //0-3年   4-5月  6-7日
    int appointment[];  //0-3年   4-5月  6-7日  8-9时  10-11分
    int age_grade;      //0-7  ，0 first
    int risk_grade;     //0-3 0 no risk, 1 low risk, 2 middle risk,3 high risk

}

#endif /* listkmp_h */

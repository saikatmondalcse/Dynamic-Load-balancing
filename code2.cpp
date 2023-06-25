#include<bits/stdc++.h>
using namespace std;
#define all(x) (x).begin(), (x).end()
#define pb push_back
#define ff first
#define ss second
#define sz(x) ((int)(x).size())
typedef long long ll;

#ifndef ONLINE_JUDGE
#define deb(x) cout<<#x<<" "<<x<<"\n"
#define deb2(x,y) cout<<"[ "<<#x<<"="<<x<<","<<#y<<"="<<y<<" ]\n"
#define deba(x,n) cout<<#x<<": [ ";for(int i=0;i<n;i++){cout<<x[i]<<" ";}cout<<"]\n"; 
#define deba2(x,n,m) cout<<#x<<":\n";for(int i=0;i<n;i++){for(int j=0;j<m;j++)cout<<x[i][j]<<" ";cout<<"\n";}; 
#define debe(x) cout<<#x<<":[ ";for(auto i:x){cout<<i<<" ";}cout<<"]\n"; 
#else
#define deb(x) 
#define deb2(x,y) 
#define deba(x,n) 
#define deba2(x,n,m) 
#define debe(x) 
#endif




struct Network{

    int L,G,C,cnt;
    double W1,W2,CUth=0;
    vector<vector<vector<double>>>CU;
    vector<vector<vector<int>>>y;
    vector<vector<vector<int>>>ytemp;
    vector<vector<vector<double>>>PER;
    vector<vector<double>>Dist;
    vector<double>IRate;
    int InterGatewaySwitch=0,IntraGatewaySwitch=0;
    vector<array<int,2>>connections;
    double of=0 ,opk=0;
    vector<array<double,2>>pointsl={{0,0},{20,160},{50,25},{90,100},{160,50},{190,130},{220,40},{150,180},{250,170},{270,70}};
    vector<array<double,2>>pointsg={{0,0},{100,150},{120,70},{240,130}};
  

    Network(int l,int g,int c,double w1,double w2)
    {
        L=l,G=g,C=c,cnt=0;
        CU=vector<vector<vector<double>>>(l+1,vector<vector<double>>(g+1,vector<double>(c+1)));
        y=vector<vector<vector<int>>>(l+1,vector<vector<int>>(g+1,vector<int>(c+1)));
        ytemp=vector<vector<vector<int>>>(l+1,vector<vector<int>>(g+1,vector<int>(c+1)));
        PER=vector<vector<vector<double>>>(l+1,vector<vector<double>>(g+1,vector<double>(c+1)));
        Dist=vector<vector<double>>(l+1,vector<double>(g+1));
        IRate=vector<double>(l+1);
        connections=vector<array<int,2>>(l+1);
        W1=w1,W2=w2;
    }   

    void setDist()
    {
        for(int i=1;i<=L;i++)
        {
            for(int j=1;j<=G;j++)
            {
                array<double,2>p1=pointsl[i],p2=pointsg[j];
                double d=(p1[0]-p2[0])*(p1[0]-p2[0])+(p1[1]-p2[1])*(p1[1]-p2[1]);
                d=sqrt(d);
                this->Dist[i][j]=d;
            }
        }
    }

    void setrandom()
    {
        // mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        

        for(int i=1;i<=L;i++)
        {
            for(int j=1;j<=G;j++)
            {

                for(int c=1;c<=C;c++)
                {
                    // int dr=rng()%10000;
                    // dr=abs(dr)+1;
                    // int nr=rng()%dr;
                    // nr=abs(nr);
                    // double x=nr*1.0/(2*dr);

                    // PER[i][j][c]=x;
                    double p=(IRate[i]/(1-PER[i][j][c]));
                    CU[i][j][c]=p;
                }
            }
        }
        
    }

   
    array<double,2> cal_f()
    {
        double mxCUjc=0;
        for(int j=1;j<=G;j++)
        {
            for(int c=1;c<=C;c++)
            {
                double CUjc=0;
                for(int i=1;i<=L;i++)
                {
                    CUjc+=(this->ytemp[i][j][c]*this->CU[i][j][c]);
                }
                mxCUjc=max(mxCUjc,CUjc);
            }
        }
        double X=mxCUjc;
        double Y=0,Z=0;
        if(this->cnt==0)
        {
            return {X,X};
        }
        vector<int>gateways_before(L+1);
        vector<int>gateways_after(L+1);
        for(int i=1;i<=L;i++)
        {
            for(int j=1;j<=G;j++)
            {
                int s1=0;
                int s2=0;
                for(int c=1;c<=C;c++)
                {
                    s1+=this->y[i][j][c];
                    s2+=this->ytemp[i][j][c];
                }
                if(s1)
                    gateways_before[i]=j;            
                if(s2)
                    gateways_after[i]=j;
            }
        }

        for(int i=1;i<=L;i++)
        {
            if(gateways_after[i]!=gateways_before[i])
            {
                Y++;
            }
            Z+=(Dist[i][gateways_after[i]]-Dist[i][gateways_before[i]]);
        }
        double f=X+(Y/this->W1)+(Z/this->W2);

        return {f,X};   
    }


    void generate_all(int l=1)
    {
        if(l==this->L+1)
        {
            array<double,2> curr=this->cal_f();
            if(curr[0]<of)
            {
                of=curr[0];
                opk=curr[1];

                for(int i=1;i<=L;i++)
                {
                    for(int j=1;j<=G;j++)
                    {
                        for(int c=1;c<=C;c++)
                        {
                            if(ytemp[i][j][c])
                            {
                                this->connections[i]={j,c};
                            }
                        }
                    }
                }
            }
            return;
        }

        for(int j=1;j<=G;j++)
        {
            for(int c=1;c<=C;c++)
            {

                this->ytemp[l][j][c]=1;
                generate_all(l+1);
                this->ytemp[l][j][c]=0;       
            }
        }    
    }

    void setoptimalconnection()
    {
        vector<array<int,2>>gateway(L+1);
        for(int i=1;i<=L;i++)
        {
            for(int j=1;j<=G;j++)
            {
                for(int c=1;c<=C;c++)
                {
                    if(y[i][j][c])
                        gateway[i]={j,c};
                }
            }
        }

        if(this->cnt!=0)
        {
            for(int i=1;i<=L;i++)
            {
                if(gateway[i][0]!=connections[i][0]){
                    cout<<"LM "<<i<<" changes gateway from "<<gateway[i][0]<<" to  "<<connections[i][0]<<endl;
                    this->InterGatewaySwitch++;
                }
                if(gateway[i][0]==connections[i][0] && gateway[i][1]!=connections[i][1]){
                    cout<<"LM "<<i<<" changes channel from "<<gateway[i][1]<<" to  "<<connections[i][1]<<endl;
                    this->IntraGatewaySwitch++;
                }
            }
        }
        for(int i=1;i<=L;i++)
        {
            for(int j=1;j<=G;j++)
            {
                for(int c=1;c<=C;c++)
                {
                    if(j==this->connections[i][0] && c==this->connections[i][1])
                        this->y[i][j][c]=1;
                    else
                        this->y[i][j][c]=0;
                }
            }
        }



    }
    void updateCUth() // from k*
    {
        if(CUth<opk)
        {
            CUth=opk+5;
        }
        else
        {
            CUth=CUth*0.95;
            if(CUth<opk)
                CUth=opk+5;
        }
    }

    void execute()
    {
        if(this->cnt!=0)
        {
            cout<<"CUBE executed"<<endl;
        }
        this->of=1e18;
        generate_all();
        setoptimalconnection();
        updateCUth();
        this->cnt++;
    }



    void changeInputRate(int i,double add)
    {
        double prev=this->IRate[i];
        this->IRate[i]+=add;

        for(int j=1;j<=G;j++)
        {
            for(int c=1;c<=C;c++)
            {
                CU[i][j][c]=(this->IRate[i])/(1-this->PER[i][j][c]);
            }
        }
    }
    void printPER()
    {
        for(int i=1;i<=L;i++)
        {
            cout<<"LM "<<i<<endl;
            for(int j=1;j<=G;j++)
            {
                for(int c=1;c<=C;c++)
                {
                    cout<<PER[i][j][c]<<" ";
                }
                cout<<endl;
            }
            cout<<endl;
        }
    }

    void CheckChannelLoad()
    {
        if(this->cnt==0)
            execute();
        else
        {
            bool flag=false;
            for(int j=1;j<=G;j++)
            {
                for(int c=1;c<=C;c++)
                {
                    if(flag)
                        return;
                    double CUjc=0;
                    for(int i=1;i<=L;i++)
                    {   
                        CUjc+=y[i][j][c]*CU[i][j][c];
                    }       
                    if(CUjc>CUth)
                    {
                        this->execute();
                        flag=true;
                    }
                }       
            }
        }
    }
    void printchannelload()
    {
        cout<<"*******************************************************************************"<<endl;

        for(int j=1;j<=this->G;j++)
        {
            for(int c=1;c<=this->C;c++)
            {
                double CUjc=0;
                for(int i=1;i<=L;i++)
                {
                    CUjc+=y[i][j][c]*CU[i][j][c];
                }
                cout<<"Gateway "<<j<<" channel " <<c<<" load is "<<CUjc<<endl;
            }
        }

    }

    void printconnection()
    {
        cout<<"*******************************************************************************"<<endl;
        cout<<"CUth : "<<this->CUth<<" K* : "<<opk<<" of : "<<of<<endl;
        cout<<"No of InterGatewaySwitch: "<<this->InterGatewaySwitch<<" , No of IntraGatewaySwitch: "<<this->IntraGatewaySwitch<<endl;
        for(int i=1;i<=L;i++)
        {
            cout<<"LM "<<i<<" [input data rate : "<<IRate[i]<<"] "<< "is connected to gateway "<<this->connections[i][0]<<" through channel "<<this->connections[i][1]<<" Estimated load :"<<CU[i][connections[i][0]][connections[i][1]]<<endl;
        }
        cout<<"*******************************************************************************"<<endl;
    }



};










int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);


    Network Prev(8,3,2,0.02,10);
    Network NP(8,3,2,0.02,10);


        NP.PER[1]=  {
                        {0,0,0},
                        {0,0.0132653 ,0.111222},
                        {0,0.2312558 ,0.131881},
                        {0,0.00243524 ,0.144165}
                    };        
        NP.PER[2]=  {
                                {0,0,0},
   
                        {0,0.0619891 ,0.117299},
                        {0,0.0144542, 0.10245},
                        {0,0.113772 ,0.148583}
        };        
        NP.PER[3]=  {                          {0,0,0},
 
                        {0,0.177966, 0.158764},
                        {0,0.050843 ,0.0201083},
                        {0,0.25519 ,0.143678}
        };        
        NP.PER[4]=  {                           {0,0,0},

                        {0,0.082996, 0.259645},
                        {0,0.109751 ,0.0592008},
                        {0,0.275177 ,0.211348}
        };        
        NP.PER[5]=  {                           {0,0,0},

                        {0,0.156874 ,0.231484},
                        {0,0.101009 ,0.153153},
                        {0,0.167644 ,0.17501}
        };        
        NP.PER[6]=  {                           {0,0,0},

                        {0,0.193517 ,0.453112},
                        {0,0.09188 ,0.17236},
                        {0,0.199735 ,0.229118}
        };        
        NP.PER[7]=  {                           {0,0,0},

                        {0,0.294833 ,0.112185},
                        {0,0.172833 ,0.159592},
                        {0,0.0655949 ,0.202094}
        };        
        NP.PER[8]=  {                           {0,0,0},

                        {0,0.301972 ,0.0928854},
                        {0,0.232666, 0.187916},
                        {0,0.081038 ,0.137234}
        };





    // NP.CheckChannelLoad();

    NP.y[1][3][1]=1;
    NP.y[2][1][1]=1;
    NP.y[3][2][1]=1;
    NP.y[4][1][1]=1;
    NP.y[5][3][2]=1;
    NP.y[6][2][2]=1;
    NP.y[7][3][1]=1;
    NP.y[8][1][2]=1;
    NP.connections={{0,0},{3,1},{1,1},{2,1},{1,1},{3,2},{2,2},{3,1},{1,2}};
    NP.CUth=1164.9,NP.opk=1159.9,NP.of=1159.9;
    NP.setrandom();
    NP.setDist();
    NP.cnt=20;
    NP.IntraGatewaySwitch=10;
    NP.InterGatewaySwitch=4;
    NP.CheckChannelLoad();
    // int p=1;
    while(1)
    {
        cout<<"1. printconnection details"<<endl;
        cout<<"2. change input rate"<<endl;
        cout<<"3. printchannelload"<<endl;  
        cout<<"4. printper"<<endl;  
        cout<<"5. exit"<<endl;
        cout<<"> "<<flush;
        int op;
        cin>>op;
        if(op==-1)
        {
            NP=Prev;
        }
        else if(op==1)
        {
            NP.printconnection();
        }
        else if(op==2)
        {
            Prev=NP;
            cout<<"Choose LM and additional input rate"<<endl;
            cout<<"> "<<flush;
            int i;
            double add;
            cin>>i>>add;
            if(i>NP.L )
                continue;
            NP.changeInputRate(i,add);
            NP.CheckChannelLoad();
        }
        else if(op==3)
        {
            NP.printchannelload();
        }
        else if(op==4)
        {
            NP.printPER();
        }
        else
        {
            continue;
        }
        
    }


    return 0;
}

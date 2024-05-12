#include <Headers/Graph.h>
#include <Headers/User_Interface.h>

int main()
{

    cout<<"Test"<<endl;
  graph g;

    User_Inrerface mumap;
    mumap.AddCity("alex",g);
    mumap.AddCity("cairo",g);
    mumap.AddCity("zefta",g);
    mumap.AddCity("CAIro",g);
    mumap.AddEdge("cairo","alex",180,g);
    mumap.AddEdge("cairo","zefta",320,g);
    mumap.AddEdge("cairo","zefta",320,g);
    mumap.DeleteCity("alex",g);
    mumap.DeleteEdge("cairo","zefta",g);

//mumap.TraverDfs("cairo",g);



    return 0;
}

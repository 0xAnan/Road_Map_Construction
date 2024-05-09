#include <Headers/Graph.h>
#include <Headers/User_Interface.h>

int main()
{

    cout<<"Test"<<endl;
    /*graph test;
    test.addcity("alex");
    test.addcity("cairo");
    test.addcity("zefta");
    test.addedge("cairo","alex",180);
    test.addedge("cairo","zefta",150);
    test.deletecity("alex");
    test.delete_edge("cairo","zefta");
*/
    UI mumap;
    mumap.disp_AddCity("alex");
    mumap.disp_AddCity("cairo");
    mumap.disp_AddCity("zefta");
    mumap.AddEdge("cairo","alex",180);
    mumap.AddEdge("cairo","zefta",320);
    mumap.DeleteCity("alex");
    mumap.DeleteEdge("cairo","zefta");






    

    return 0;
}

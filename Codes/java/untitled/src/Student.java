public class Student {
    private int id;
    private String name;
    private float marks;
    public Student(int id, String name, float marks){
        this.id = id;
        this.name = name;
        this.marks = marks;
    }
    @Override
    public boolean equals(Object obj) {
        Student otherStudent = (Student) obj;
        if(this.name == otherStudent.name){

            return true;
        } else{
            return false;
        }
    }
}
#ifndef __VISITABLE_H
#define __VISITABLE_H

class Visitor;


class Visitable {
public:
        virtual void accept(Visitor* visitor) = 0;
};



#endif // !__VISITABLE_H




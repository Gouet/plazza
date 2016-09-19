//
// Created by gaspar-q on 20/04/16.
//

#ifndef CPP_PLAZZA_PIPE_H
#define CPP_PLAZZA_PIPE_H

class Pipe
{
public:
    Pipe();
    Pipe(Pipe const &);
    ~Pipe();
    Pipe    &operator=(Pipe const &);

public:
    void    linkInTo(int fd);
    void    linkOutTo(int fd);
    int     getIn() const;
    int     getOut() const;
    void    resetIn();
    void    resetOut();
    void    reset();
    void    closeIn();
    void    closeOut();
    std::string readPipe() const;
    int         writePipe(std::string const &) const;

private:
    int pipeIn;
    int pipeOut;
    int saveIn;
    int saveOut;
};

#endif //CPP_PLAZZA_PIPE_H

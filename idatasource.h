#ifndef IDATASOURCE_H
#define IDATASOURCE_H

class IDataSource
{
public:
    virtual ~IDataSource();

    virtual std::vector<char> getData(int offset, int size) = 0;
};

#endif // IDATASOURCE_H

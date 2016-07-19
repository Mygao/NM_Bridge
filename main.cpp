#include "include/rsi_server.h"

#include <iostream>

int main()
{
    RSIServer rsi_server(60080);

    rsi_server.Run();

    return 0;
}

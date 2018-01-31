CC=g++ 

CFLAGS=-Wall -Wextra -g -pthread

NTLFLAGS= -std=c++11 -I /users/cosic/aaly/NTL/include -I ./
LIBS= -L /users/cosic/aaly/NTL/lib -lntl -lgmp

edgeRuntime:main.o StandardShare.o StandardPlayer.o ShamirSharesEngine.o SocketPool.o AuctionResponse.o Constants.o ReducedBid.o StandardBid.o EngineBuffers.o LocationalAuction.o ShamirGenerator.o SharesManager.o PushRelabel.o SharesListener.o List.o BellmanFord.o List.o Matrix.o MathUtil.o ShareUtil.o LocationalAuctionUtil.o PermutationMatrix.o UdpSocket.o BatcherMergeSort.o BidUtil.o StandardShareDao.o FileUtil.o IStandardSocket.o GeneralAuctionResponse.o GeneralMarketAuction.o GeneralMarketBid.o GeneralMarketAuctionUtil.o
	$(CC) $(CFLAGS) $(NTLFLAGS) $^ -o $@ $(LIBS)
%.o:%.cpp
	$(CC) $(CFLAGS) $(NTLFLAGS) -c $< $(LIBS) 


clean:
	rm -f edgeRuntime *.o

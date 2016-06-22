//
//  SharesListener.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 29/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#ifndef edgeRuntime_SharesListener_h
#define edgeRuntime_SharesListener_h

//Custom Headers
#include "StandardPlayer.h"
#include "StandardShareDao.h"
#include "List.h"


namespace Listeners
{
    /**
     * @class SharesListener
     * @ingroup Listeners
     * @brief
     * In charge of incoming comunications.
     * @details The class uses DAOS to constantly intercept all incoming transmissions and store them in such a way the application can use them.
         There is only one listener per application.
     * @todo 
       Build a private constructor for the class
     * @author Abdelrahaman Aly
     * @date  2014/02/11
     * @version 0.0.1.15
     */
    class SharesListener
    {
    private:
        //singleton instance of the listener
        static SharesListener * listener_;
        
        /**
         * @brief Method executed by any Listener thread. Designed to be use on multithreading under Thread Libraries.
         * @details Method has to added to a thread on any configuration phase of the SmcEngines::ShamirSharesEngine.
            <ul>
             Shares are identified as well, if their parameters are -1 then it understands is a sync share. All but the first Sync Shares are discarded.
            <\ul>
         
         * @param threadid necessary for thread libraries.
         * @return void pointer
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Pooling and on exception control return 0
         </ul>
         */
        static void * listenerProcess(void *threadid);

    public:
        //status variable
        static bool alive_;
        //unique instance of the corresponding DAO
        static Daos::StandardShareDao * standardShareDao_;
        //Unique list of players, the application is listening to.
        static Utils::List<Players::StandardPlayer> * players_;
        
        //Unique player the listener is listening for
        static Players::StandardPlayer * player_;
        
        /**
         * @brief Specialized Constructor
         * @details  Allows to set state variables
         * @param player Unique player the listener is listening for
         * @param payers Utils::List of players, listenter is listening to.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Pooling and on exception control return 0
         </ul>
         */
        SharesListener(Players::StandardPlayer * player, Utils::List<Players::StandardPlayer> * players);

        /**
         * @brief Responsible to start the listener thread and execute validation process.
         * @details Method fullfils two esential tasks: 
           <ul>
               <li>Configure Runtime (Specially Cleaning Buffers) to be ready for execution.
               <li>Coordinate the startup of all listener, provide the process, data and time needed for that matter.
               <li> Notify master trhead of success or faliure on these tasks
           </ul>
         * @return The procedure status.
         * @retval status The following are the procedure status:
         <ul>
         <li>1= success
         <li>0=faliure
         </ul>
         * @exception NA
         * @todo
         <ul>
         <li>Modify Data Buffer sizes such that they grow in function of the number of players
         <li>Build Exceptions for Pooling and on exception control return 0
         </ul>
         @note Once this method has been called, the only way the application will proced to next programaticalschedule tasks would be if synchronization process has finish its execution.
         */
        int startListner();
        
        /**
         * @brief Bean Function\Property.
         * @details Gets thread status.
         * @return The Thread status.
         * @retval status The following are the Thread status:
         <ul>
         <li>true= success
         <li>false=faliure
         </ul>
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Pooling and on exception control return 0
         </ul>
         */
        bool isAlive();
        
        /**
         * @brief In charge of stoping Listener Thread
         * @details Securely stops the thread to avoid risks on information damage.
         * @return The procedure status.
         * @retval status The following are the procedure status:
         <ul>
         <li>1= success
         <li>0=faliure
         </ul>
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Pooling and on exception control return 0
         </ul>
         */
        int stopListener();
        
        /**
         * @brief It allows the user class to retreive singleton instance of the listener, in case there is none it will use the parameters to create a new and unique one.
         * @param player is the object player that needs the Listener
         * @param players pointer to list of players of the user class
         * @see SharesListener()
         */
        SharesListener * getListener(Players::StandardPlayer * player, Utils::List<Players::StandardPlayer> * players);
        
    };
    
}


#endif

import logging
import datetime


# global logger 

# logger = None

class LoggerSingleton:
    
    @staticmethod
    def initLogger(name, path):
        if name in logging.Logger.manager.loggerDict:
            logger = logging.getLogger(name)
            # print("Logger already exists")
            # print(str(logging.Logger.manager.loggerDict))
        else:
            # print("Logger does not exists, yet")
            # print(str(logging.Logger.manager.loggerDict))
            logger = logging.getLogger(name)
            logger.setLevel(logging.DEBUG)
            if not logger.handlers:
                # file_name='/cygdrive/c/Retro/CROSS-LIB/logs/log_'+datetime.datetime.now().strftime("%Y-%m-%d_%H_%M_%S")+'.log'
                
                # file_name=path+'/log_'+name+'_'+datetime.datetime.now().strftime("%Y-%m-%d_%H_%M_%S")+'.log'
                file_name=path+'/log.log'
                file_handler = logging.FileHandler(file_name,mode='a+', encoding="utf-8")
                formatter = logging.Formatter('%(asctime)s - %(filename)s:%(lineno)s - %(message)s')
                file_handler.setFormatter(formatter)
                logger.addHandler(file_handler)
        return logger

        
    # @staticmethod
    # def initConsoleLogger(name):
        # if name in logging.Logger.manager.loggerDict:
            # logger = logging.getLogger(name)
        # else:
            # logger = logging.getLogger(name)
            # logger.setLevel(logging.DEBUG)
            # if not logger.handlers:
                # console_handler = logging.StreamHandler()
                # formatter = logging.Formatter('%(asctime)s - %(filename)s:%(lineno)s - %(message)s')
                # console_handler.setFormatter(formatter)
                # logger.addHandler(console_handler)
        # return logger  
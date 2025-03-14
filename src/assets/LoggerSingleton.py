import logging
import datetime


global logger 

# logger = None

class LoggerSingleton:
    
    @staticmethod
    def initLogger():
        logging.basicConfig(filename='../logs/log_'+datetime.datetime.now().strftime("%Y-%m-%d_%H_%M_%S")+'.log', level = logging.DEBUG)

    @staticmethod
    def getLogger():
        return logging.getLogger(__name__)
            
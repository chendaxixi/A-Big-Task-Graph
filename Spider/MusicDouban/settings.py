# Scrapy settings for MusicDouban project
#
# For simplicity, this file contains only the most important settings by
# default. All the other settings are documented here:
#
#     http://doc.scrapy.org/topics/settings.html
#

BOT_NAME = 'MusicDouban'
BOT_VERSION = '1.0'

SPIDER_MODULES = ['MusicDouban.spiders']
NEWSPIDER_MODULE = 'MusicDouban.spiders'
USER_AGENT = 'Mozilla/5.0 (Windows NT 6.1; rv:7.0) Gecko/20100101 Firefox/7.0'

DOWNLOAD_DELAY = 2.5
COOKIES_ENABLED = False

DOWNLOADER_MIDDLEWARES = {
        'scrapy.contrib.downloadermiddleware.useragent.UserAgentMiddleware' : None,
        'MusicDouban.spiders.rotate_useragent.RotateUserAgentMiddleware' :400
    }

DEFAULT_ITEM_CLASS = 'MusicDouban.items.Singer'

ITEM_PIPELINES = {'MusicDouban.pipelines.MusicdoubanPipeline': 1}

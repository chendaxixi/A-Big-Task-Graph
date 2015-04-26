'''
Created on 2015-1-4

@author: Administrator
'''

from scrapy.spider import BaseSpider
from scrapy.http import Request
from scrapy.selector import HtmlXPathSelector

from MusicDouban.items import Singer

import random

class MusicSpider(BaseSpider):
    name = "Music"
    allowed_domains = ["music.douban.com"]
    start_urls = ["http://music.douban.com/tag/%E5%8F%B0%E6%B9%BE"]
 
    def parseSinger(self, response):
        sel = HtmlXPathSelector(response)
        item = Singer()
        item["player"] = sel.select('//div[@id="info"]')[0].select("span/span/a/text()").extract()[0].encode("utf-8")
        temp = sel.select('//div[@class="content clearfix"]')
        if(len(temp) == 1):
            return
        item["other"] = [x.encode("utf-8") for x in temp[0].select("dl/dd/a/text()").extract()]
        open("result.txt", "a").write(item["player"]+"\n")
        for x in item["other"]:
            open("result.txt", "a").write(x+"\n")
        open("result.txt", "a").write("***\n")
        return item
    
    def parse(self, response):
        sel = HtmlXPathSelector(response)
       # open("url.txt", 'a').write(response.url+"\n")
        sites = sel.select('//a[@class="nbg"]')
        items = []
        
        for site in sites:
            item = site.select("@href").extract()[0]
            items.append(item)
        
        result = []
        
        for item in items:
            if(item.find(u"subject")!=-1):
                result.append(Request(item, callback=self.parseSinger))

        site = sel.select('//span[@class="next"]').select("a/@href").extract()[0]
        result.append(Request(site, callback=self.parse))
        
        return result
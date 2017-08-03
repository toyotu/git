#!/usr/bin/ruby
# -*- coding: utf-8 -*-


require "open3"

$mecab_path = "/usr/local/bin/mecab"
$dic_path = "/Users/hiroshi/t-chem/dictionary/workspace/all-chem-fixed.dic"

#mecab出力の一行
class M_line

  attr_accessor :surf, :pos, :pos2, :pos3, :pos4, :pos5, :pos6, :normal, :yomi
  def initialize(line)
    line.chomp!
    sets = line.split("\t")
    @surf = sets[0]
    lats = sets[1].split(",")
    @pos = lats[0] ||= ""
    @pos2 = lats[1] ||= ""
    @pos3 = lats[2] ||= ""
    @pos4 = lats[3] ||= ""
    @pos5 = lats[4] ||= ""
    @pos6 = lats[5] ||= ""
    @normal = lats[6] ||= @surf #nilの場合は表層形を利用
    @yomi = lats[7] ||= ""
  end

  #元の表示に戻す
  def show   
    return [@surf, [@pos, @pos2, @pos3, @pos4, @pos5, @pos6, @normal, @yomi].join(",")].join("\t")
  end

  #二つのlineを一つにまとめる（名詞＋する）
  def merge(lat_m_line)
    self.surf = self.surf + lat_m_line.surf
    self.pos = lat_m_line.pos
    self.pos2 = lat_m_line.pos2
    self.pos3 = lat_m_line.pos3
    self.pos4 = lat_m_line.pos4
    self.pos5 = lat_m_line.pos5
    self.pos6 = self.pos6 + lat_m_line.pos6
    self.normal = self.normal + lat_m_line.normal
    self.yomi = self.yomi + lat_m_line.yomi
    return self
  end

end


class Mecab
  attr_reader :result, :with_EOS
  def initialize(input)
    command = $mecab_path
    command = $mecab_path + " -u " + $dic_path if  $dic_path != nil
    Open3.popen3(command){|stdin, stdout, stderr, thread|
      stdin.puts(input)
      stdin.close
      stdout.set_encoding("UTF-8")
      @with_EOS = stdout.collect{|item| item}
    }
    @result = @with_EOS.select{|item| item != "EOS\n"}.collect{|line| M_line.new(line)}
    return self
  end

  #指定された品詞のみを出力する(Array)
  def select(pos)    
    @result = @result.select{|line| line.pos == pos}
    return self
  end

  #解析結果の表層形を全て表示する
  def surface
    return @result.collect{|line| line.surf}  
  end

  #解析結果の標準形を全て表示する
  def normal
    return @result.collect{|line| line.normal}
  end
  
  #サ変接続名詞と「する」をまとめる
  def n_merge
    del_nums = []
    arr = []
    count = 0
    d_flag = 0 #mergeした時点で立てて、次のやつを捨てる
    arr = @result.each_cons(2).to_a.collect{|former, latter|
      if d_flag == 1
        d_flag = 0
        nil #前でmergeしたらnilにする
      elsif former.pos == "名詞" && latter.pos == "動詞" && latter.pos5 == "サ変・スル"
        d_flag = 1
        del_nums << count
        former.merge(latter)
      else
        former
      end
    }

    if d_flag == 1
      @result = arr.compact
    else
      @result = arr.compact << @result[-1]
    end
    
    return self 
  end
  
  #しやすい、にくいをまとめる  
  def adj_merge
    del_nums = []
    arr = []
    count = 0
    d_flag = 0 #mergeした時点で立てて、次のやつを捨てる
    arr = @result.each_cons(2).to_a.collect do |former, latter|
      if d_flag == 1
        d_flag = 0
        nil #前でmergeしたらnilにする
      elsif former.pos == "動詞" && latter.pos == "形容詞" && latter.pos2 == "非自立"
        d_flag = 1
        del_nums << count
        former.merge(latter)
      else
        former
      end
    end

    if d_flag == 1
      @result = arr.compact
    else
      @result = arr.compact << @result[-1]
    end
    return self 
  end
  
  def show
    return @result.collect{|line| line.show}.join("\n")
  end
end

<?php
/**
 * @file
 * This is the template file for the object page for person objects.
 */
?>
<div class="islandora-object islandora">

  <div class="islandora-object-content">
    <div class="islandora-object-image">
      <?php if (isset($variables['tn'])): ?>
        <img src="<?php print $variables['tn']; ?>"/>
      <?php endif; ?>
    </div>
    <div class="islandora-object-metadata">
      <?php if (isset($variables['metadata'])): ?>
        <?php print $variables['metadata']; ?>
      <?php endif; ?>
    </div>
     <div class="islandora-external-links islandora">
        <h3 class="islandora-external-links"><?php print t('External Links'); ?></h3>
        <div class="islandora-external-links-leblanc">
            <?php if($wiki_page): ?>            
            <a href="<?php print $variables['wiki_page']; ?>"><?php print t('Wikipedia'); ?></a>
            <?php endif; ?>   
        </div>
        <div class="islandora-external-links-leblanc">
        <?php if($leblanc): ?>            
            <a href="<?php print $variables['leblanc']; ?>"><?php print t('Edward T. LeBlanc Bibliography'); ?></a>
        <?php endif; ?>   
        </div>
                <div class="islandora-external-links-viaf">
        <?php if($viaf): ?>            
            <a href="<?php print $variables['viaf']; ?>"><?php print t('Virtual International Authority File'); ?></a>
        <?php endif; ?>   
        </div>
    </div>
	<div class="islandora-related-objects islandora">
	   <div class="islandora-related-content-wrapper clearfix">
	       <?php if($related): ?>
	       <h3 class="bio"><?php print t('Related identities in the collection'); ?></h3>
    	   <div>
    	       <ul>
    	       <?php foreach ($related as $obj): ?>
    	           <li><?php print l($obj->label, "islandora/object/{$obj->id}"); ?></li>
    	       <?php endforeach; ?>
    	       </ul>
           </div>
           <?php endif; ?>
       </div>
    </div>
    <div class="islandora-object-citations">
    <?php if (isset($variables['recent_citations'])): ?>
      <?php print $variables['recent_citations']; ?>
    <?php endif; ?>
    <?php if (isset($variables['rss_feed'])): ?>
      <?php print $variables['rss_feed']; ?>
    <?php endif; ?>
    </div>
   </div>
    <div class="islandora-object-wikipedia">
    <?php if (isset($variables['abstract'])): ?>
      <p><?php print $variables['abstract']; ?></p>
    <?php endif; ?>
    </div>
    <div class="islandora-object-biography">
    <h3 class="bio"><?php print t('Biography'); ?></h3>
    <?php if (isset($variables['biography'])): ?>
      <p><?php print $variables['biography']; ?></p>
    <?php endif; ?>
    </div>
</div>